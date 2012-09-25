#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "nvram.h"
#include "rama.h"
#include "md5.h"

#include <linux/wireless.h>


typedef struct rm_srv_state {
	int state; //0:init, 1:waiting pw, 2:login
	char nonce[RM_NONCE_LEN];
	char cid[6]; //client id
} rm_srv_state;
rm_srv_state g_state = {0};

int rm_skfd = 0;



//this should be the same as goahead/src/internet.c
static char* getLanIfName(void)
{
	char *mode = nvram_get(RT2860_NVRAM, "OperationMode");
	static char *if_name = "br0";

	if (NULL == mode)
		return if_name;
	if (!strncmp(mode, "0", 2))
		if_name = "br0";
	else if (!strncmp(mode, "1", 2)) {
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW
		if_name = "br0";
#elif defined  CONFIG_ICPLUS_PHY && CONFIG_RT2860V2_AP_MBSS
		char *num_s = nvram_bufget(RT2860_NVRAM, "BssidNum");
		if(atoi(num_s) > 1) // multiple ssid
			if_name = "br0";
		else
			if_name = "ra0";
#else
		if_name = "ra0";
#endif
	}
	else if (!strncmp(mode, "2", 2)) {
		if_name = "eth2";
	}
	else if (!strncmp(mode, "3", 2)) {
		if_name = "br0";
	}
	return if_name;
}

static char* getWanIfName(void)
{
	char *mode = nvram_get(RT2860_NVRAM, "OperationMode");
	static char *if_name = "br0";

	if (NULL == mode)
		return if_name;
	if (!strncmp(mode, "0", 2))
		if_name = "br0";
	else if (!strncmp(mode, "1", 2)) {
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW
		if_name = "eth2.2";
#else /* MARVELL & CONFIG_ICPLUS_PHY */
		if_name = "eth2";
#endif
	}
	else if (!strncmp(mode, "2", 2))
		if_name = "ra0";
	else if (!strncmp(mode, "3", 2))
		if_name = "apcli0";
	return if_name;
}

static int get_my_mac(char *if_hw)
{
	struct ifreq ifr;
	int skfd;
	char *ptr;

	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

#ifdef CONFIG_ICPLUS_PHY
	strncpy(ifr.ifr_name, getWanIfName(), IFNAMSIZ);
#else
	strncpy(ifr.ifr_name, getLanIfName(), IFNAMSIZ);
#endif
	if(ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
		perror("ioctl");
		return -1;
	}

	ptr = (char *)&ifr.ifr_addr.sa_data;
	//printf("mac= %02X:%02X:%02X:%02X:%02X:%02X\n",
	//		(ptr[0] & 0377), (ptr[1] & 0377), (ptr[2] & 0377),
	//		(ptr[3] & 0377), (ptr[4] & 0377), (ptr[5] & 0377));
	memcpy(if_hw, ptr, 6);

	close(skfd);
	return 0;
}

static int get_my_ip(char *ip)
{
	struct ifreq ifr;
	int skfd = 0;
	char *p;

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

#ifdef CONFIG_ICPLUS_PHY
	strncpy(ifr.ifr_name, getWanIfName(), IFNAMSIZ);
#else
	strncpy(ifr.ifr_name, getLanIfName(), IFNAMSIZ);
#endif
	if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) {
		perror("ioctl");
		return -1;
	}
	p = (char *)&((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;
	//printf("ip= %d.%d.%d.%d\n", p[0]&0377, p[1]&0377, p[2]&0377, p[3]&0377);
	memcpy(ip, p, 4);

	close(skfd);
	return 0;
}

static int get_ssid(char *ssid)
{
	struct iwreq wrq;
	int skfd = 0;
	char *mode = nvram_get(RT2860_NVRAM, "OperationMode");
	char *ec = nvram_get(RT2860_NVRAM, "ethConvert");

	if (!strncmp(mode, "2", 2) && strncmp(ec, "1", 2)) {
		strncpy(ssid, "<station>", 10);
		return 0;
	}

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	strncpy(wrq.ifr_name, "ra0", 4);
	wrq.u.essid.pointer = ssid;
	wrq.u.essid.length = IW_ESSID_MAX_SIZE + 1;
	wrq.u.essid.flags = 0;

	if (ioctl(skfd, SIOCGIWESSID, &wrq) < 0) {
		perror("ioctl");
		return -1;
	}
	//printf("ssid= %s\n", (char *)wrq.u.essid.pointer);

	close(skfd);
	return 0;
}

int send_reply(unsigned short sin_port, rm_msg *m)
{
	struct sockaddr_in c_sa;

	c_sa.sin_family = AF_INET;
	c_sa.sin_port = sin_port;
	c_sa.sin_addr.s_addr = INADDR_BROADCAST;
	memset(c_sa.sin_zero, '\0', sizeof(c_sa.sin_zero));
	//printf("port: %d\n", ntohs(sin_port));

	if (rm_msg_check(m) != 0)
		return -1;
	//printf("sending reply::\n");
	//rm_print_msg(m);
	if (sendto(rm_skfd, m->h, m->h->len, 0, (struct sockaddr *)&c_sa, sizeof(c_sa)) == -1) {
		perror("sendto");
		return -1;
	}
	return 0;
}

int handle_query(unsigned short sin_port, rm_msg *q)
{
	rm_msg r;
	rm_tlv *t;
	int len;
	char mac[6], ip[4], ssid[RM_MAX_SSID_LEN];

	memset(mac, '\0', 6);
	memset(ip, '\0', 4);
	memset(ssid, '\0', RM_MAX_SSID_LEN);
	get_my_mac(mac);
	get_my_ip(ip);
	get_ssid(ssid);

	if (0 != rm_msg_init(&r, RM_INFO))
		return -1;

	t = RMM_TLV(q);
	len = RMM_LEN(q);
	switch (t->t) {
		case RM_Q_ALL:
			if (!RMT_OK(t, len))
				break;
			if (mac == NULL && ip == NULL && ssid == NULL)
				break;
			if (0 != rm_append_tlv(&r, RM_I_MAC, 6, mac))
				break;
			if (0 != rm_append_tlv(&r, RM_I_IP, 4, ip))
				break;
			if (0 != rm_append_tlv(&r, RM_I_SSID, strlen(ssid), ssid))
				break;
			send_reply(sin_port, &r);
			break;
	}
	rm_msg_fini(&r);
	return 0;
}

int handle_login(unsigned short sin_port, rm_msg *lo)
{
	rm_msg r;
	rm_tlv *t;
	int len, fd;
	char sid[6], mac[6];

	if (0 != rm_msg_init(&r, RM_STATUS))
		return -1;

	t = RMM_TLV(lo);
	len = RMM_LEN(lo);
	switch (t->t) {
		case RM_L_SID:
			if (RMT_OK(t, len))
				memcpy(sid, t->v, 6);
			get_my_mac(mac);
			if (memcmp(sid, mac, 6)) //another server exists?
				break;

			//create a nonce tlv
			r.h->type = RM_CHALLENGE;
			fd = open("/dev/urandom", O_RDONLY);
			if (fd == -1)
				break;
			len = read(fd, g_state.nonce, RM_NONCE_LEN);
			if (len == -1) {
				close(fd);
				break;
			}
			if (0 != rm_append_tlv(&r, RM_L_NONCE, RM_NONCE_LEN, g_state.nonce)) {
				close(fd);
				break;
			}

			g_state.state = 1;
			send_reply(sin_port, &r);
			close(fd);
			break;
		default:
			g_state.state = 0;
			if (0 != rm_append_tlv(&r, RM_S_UNKNOWN_TLV_TYPE, 17, "Unknown TLV type"))
				break;
			send_reply(sin_port, &r);
	}
	rm_msg_fini(&r);
	return 0;
}

int handle_passwd(unsigned short sin_port, rm_msg *p)
{
	rm_msg r;
	rm_tlv *t;
	int len, fd;
	char hash[16];

	//this is not for me
	if (g_state.state != 1)
		return 1;

	if (0 != rm_msg_init(&r, RM_STATUS))
		return -1;

	t = RMM_TLV(p);
	len = RMM_LEN(p);
	switch (t->t) {
		case RM_L_HASH:
			if (!RMT_OK(t, len))
				break;
			if (md5_hash(g_state.nonce,
					nvram_bufget(RT2860_NVRAM, "Login"),
					nvram_bufget(RT2860_NVRAM, "Password"),
					hash) != 0)
				break;

			//compare the hash
			if (memcmp(hash, t->v, 16)) {
				if (0 != rm_append_tlv(&r, RM_S_WRONG_PW, 28, "Wrong user name or password"))
					break;
				send_reply(sin_port, &r);
				break;
			}

			g_state.state = 2;
			if (0 != rm_append_tlv(&r, RM_S_SUCCESS, 0, NULL))
				break;

			//create a client id
			fd = open("/dev/urandom", O_RDONLY);
			len = read(fd, g_state.cid, 6);
			if (0 != rm_append_tlv(&r, RM_I_CID, 6, g_state.cid)) {
				close(fd);
				break;
			}

			send_reply(sin_port, &r);
			close(fd);
			break;
		default:
			if (0 != rm_append_tlv(&r, RM_S_UNKNOWN_TLV_TYPE, 17, "Unknown TLV type"))
				break;
			send_reply(sin_port, &r);
	}
	rm_msg_fini(&r);
	return 0;
}

static int set_my_ip(char *ip)
{
	int skfd;
	struct ifreq ifr;
	struct sockaddr_in *sa;

	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
#ifdef CONFIG_ICPLUS_PHY
	strncpy(ifr.ifr_name, getWanIfName(), IFNAMSIZ);
#else
	strncpy(ifr.ifr_name, getLanIfName(), IFNAMSIZ);
#endif

	sa = (struct sockaddr_in *)&ifr.ifr_addr;
	sa->sin_family = AF_INET;
	memcpy(&(sa->sin_addr.s_addr), ip, 4);

	if (ioctl(skfd, SIOCSIFADDR, &ifr) < 0) {
		perror("ioctl");
		return -1;
	}
	return 0;
}

static int renew_bcast_route(void)
{
	int skfd;
	struct rtentry rt;

	if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

	memset(&rt, 0, sizeof(rt));
	((struct sockaddr_in *)(&rt.rt_dst))->sin_addr.s_addr = INADDR_BROADCAST;
	((struct sockaddr_in *)(&rt.rt_dst))->sin_family = AF_INET;
	rt.rt_flags = RTF_UP | RTF_HOST;
#ifdef CONFIG_ICPLUS_PHY
	rt.rt_dev = getWanIfName();
#else
	rt.rt_dev = getLanIfName();
#endif

	if (ioctl(skfd, SIOCADDRT, &rt) < 0) {
		perror("ioctl");
		return -1;
	}
	return 0;
}

int handle_set_ip(unsigned short sin_port, rm_msg *c)
{
	rm_msg r;
	rm_tlv *t;
	int len;
	char r_cid[6], r_ip[4];

	if (0 != rm_msg_init(&r, RM_STATUS))
		return -1;

	//parse the set_ip request
	t = RMM_TLV(c);
	len = RMM_LEN(c);
	while (RMT_OK(t, len)) {
		if (t->t == RM_I_CID)
			memcpy(r_cid, t->v, 6);
		else if (t->t == RM_I_IP)
			memcpy(r_ip, t->v, 4);
		t = RMT_NEXT(t, len);
	}

	//check if logged-in
	if (g_state.state != 2 || memcmp(g_state.cid, r_cid, 6)) {
		if (0 != rm_append_tlv(&r, RM_S_UN_LOGIN, 19, "Please login first")) {
			rm_msg_fini(&r);
			return -1;
		}
		send_reply(sin_port, &r);
		rm_msg_fini(&r);
		return 0;
	}

	//modify my ip
	if (set_my_ip(r_ip) != 0) {
		if (0 != rm_append_tlv(&r, RM_S_ERROR, 22, "Server internal error")) {
			rm_msg_fini(&r);
			return -1;
		}
		send_reply(sin_port, &r);
		rm_msg_fini(&r);
		return 0;
	}
	renew_bcast_route();

	if (0 != rm_append_tlv(&r, RM_S_SUCCESS, 0, NULL)) {
		rm_msg_fini(&r);
		return -1;
	}
	send_reply(sin_port, &r);
	rm_msg_fini(&r);
	return 0;
}

static int set_ssid(char *ssid)
{
	struct iwreq wrq;
	int skfd = 0;
	char data[40];

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	strncpy(wrq.ifr_name, "ra0", 4);
	memset(data, '\0', 40);
	sprintf(data, "SSID=%s", ssid);
	wrq.u.essid.pointer = data;
	wrq.u.essid.length = strlen(data) + 1;
	wrq.u.essid.flags = 1;

	//use RTPRIV_IOCTL_SET instead of SIOCSIWESSID :(
	if (ioctl(skfd, SIOCIWFIRSTPRIV+2, &wrq) < 0) {
		perror("ioctl");
		return -1;
	}

	close(skfd);
	return 0;
}

int handle_set_ssid(unsigned short sin_port, rm_msg *c)
{
	rm_msg r;
	rm_tlv *t;
	int len;
	char r_cid[6], r_ssid[33];

	if (0 != rm_msg_init(&r, RM_STATUS))
		return -1;
	memset(r_ssid, '\0', 33);

	//parse the set_ssid request
	t = RMM_TLV(c);
	len = RMM_LEN(c);
	while (RMT_OK(t, len)) {
		if (t->t == RM_I_CID)
			memcpy(r_cid, t->v, 6);
		else if (t->t == RM_I_SSID) {
			memcpy(r_ssid, t->v, (RMT_LEN(t) > 32)? 32 : RMT_LEN(t));
		}
		t = RMT_NEXT(t, len);
	}

	//check if logged-in
	if (g_state.state != 2 || memcmp(g_state.cid, r_cid, 6)) {
		if (0 != rm_append_tlv(&r, RM_S_UN_LOGIN, 19, "Please login first")) {
			rm_msg_fini(&r);
			return -1;
		}
		send_reply(sin_port, &r);
		rm_msg_fini(&r);
		return 0;
	}

	//modify my ssid
	if (set_ssid(r_ssid) != 0) {
		if (0 != rm_append_tlv(&r, RM_S_ERROR, 22, "Server internal error")) {
			rm_msg_fini(&r);
			return -1;
		}
		send_reply(sin_port, &r);
		rm_msg_fini(&r);
		return 0;
	}

	if (0 != rm_append_tlv(&r, RM_S_SUCCESS, 0, NULL)) {
		rm_msg_fini(&r);
		return -1;
	}
	send_reply(sin_port, &r);
	rm_msg_fini(&r);
	return 0;
}

int rm_init_sock()
{
	struct sockaddr_in s_sa;
	int bcast = 1;

	if ((rm_skfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return -1;
	}

	s_sa.sin_family = AF_INET;
	s_sa.sin_port = htons(RM_PORT);
	s_sa.sin_addr.s_addr = INADDR_ANY;
	memset(s_sa.sin_zero, '\0', sizeof(s_sa.sin_zero));

	if (bind(rm_skfd, (struct sockaddr *)&s_sa, sizeof(s_sa)) == -1) {
		perror("bind");
		close(rm_skfd);
		return -1;
	}

	if (setsockopt(rm_skfd, SOL_SOCKET, SO_BROADCAST, &bcast, sizeof(bcast)) == -1) {
		perror("setsockopt (SO_BROADCAST)");
		close(rm_skfd);
		return -1;
	}

	return 0;
}

int ramad_start(void)
{
	struct sockaddr_in c_sa;
	socklen_t sa_len;
	rm_msg m;

	if (rm_init_sock() != 0)
		exit(1);

	if ((m.h = (rm_hdr *)malloc(RM_MAX_MSG_LEN)) == NULL) {
		perror("malloc");
		close(rm_skfd);
		exit(1);
	}

	//always blocking
	sa_len = sizeof(c_sa);
	while (1) {
		if (recvfrom(rm_skfd, m.h, RM_MAX_MSG_LEN, 0, (struct sockaddr *)&c_sa, &sa_len) == -1) {
			perror("recvfrom");
			break;
		}
		if (rm_msg_check(&m) != 0) {
			printf("rm_msg sanity check error\n");
			continue;
		}
		//printf("received request::\n");
		//rm_print_msg(&m);

		switch (m.h->type) {
			case RM_QUERY:
				handle_query(c_sa.sin_port, &m);
				break;
			case RM_LOGIN:
				handle_login(c_sa.sin_port, &m);
				break;
			case RM_PASSWD:
				handle_passwd(c_sa.sin_port, &m);
				break;
			case RM_SET_IP:
				handle_set_ip(c_sa.sin_port, &m);
				break;
			case RM_SET_SSID:
				handle_set_ssid(c_sa.sin_port, &m);
				break;
			default:
				break;
		}
	}
	free(m.h);
	close(rm_skfd);
	return 0;
}


