/* vi: set sw=4 ts=4 sts=4 fdm=marker: */
/*
 *	internet.c -- Internet Settings
 *
 *	Copyright (c) Ralink Technology Corporation All Rights Reserved.
 *
 *	$Id: internet.c,v 1.167.2.8 2009-04-22 01:31:35 chhung Exp $
 */

#include	<stdlib.h>
#include	<sys/ioctl.h>
#include	<net/if.h>
#include	<net/route.h>
#include    <string.h>
#include    <dirent.h>
#include	"internet.h"
#include	"nvram.h"
#include	"webs.h"
#include	"utils.h"
#include 	"firewall.h"
#include	"management.h"
#include	"station.h"
#include	"wireless.h"

#include	"linux/autoconf.h"  //kernel config
#include	"config/autoconf.h" //user config
#include	"user/busybox/include/autoconf.h" //busybox config

#ifdef CONFIG_RALINKAPP_SWQOS
#include      "qos.h"
#endif

static char wan_3g_signal[128] = "No Signal";
static char wan_3g_sim[128] = "Not Ready";

static int getMeshBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getWDSBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getWSCBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getMBSSIDBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getUSBBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getStorageBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getFtpBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getSmbBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getMediaBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getWebCamBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getPrinterSrvBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getUSBiNICBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getIgmpProxyBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getVPNBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getDnsmasqBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getGWBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getLltdBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getPppoeRelayBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getUpnpBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getRadvdBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getDynamicRoutingBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getSWQoSBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getDATEBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getDDNSBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getSysLogBuilt(int eid, webs_t wp, int argc, char_t **argv);
static int getETHTOOLBuilt(int eid, webs_t wp, int argc, char_t **argv);

static int getDhcpCliList(int eid, webs_t wp, int argc, char_t **argv);
static int getDns(int eid, webs_t wp, int argc, char_t **argv);
static int getHostSupp(int eid, webs_t wp, int argc, char_t **argv);
static int getIfLiveWeb(int eid, webs_t wp, int argc, char_t **argv);
static int getIfIsUpWeb(int eid, webs_t wp, int argc, char_t **argv);
static int getLanIp(int eid, webs_t wp, int argc, char_t **argv);
static int getLanMac(int eid, webs_t wp, int argc, char_t **argv);
static int getLanIfNameWeb(int eid, webs_t wp, int argc, char_t **argv);
static int getLanNetmask(int eid, webs_t wp, int argc, char_t **argv);
static int getWanIp(int eid, webs_t wp, int argc, char_t **argv);
static int getWanMac(int eid, webs_t wp, int argc, char_t **argv);
static int getWanIfNameWeb(int eid, webs_t wp, int argc, char_t **argv);
static int getWanNetmask(int eid, webs_t wp, int argc, char_t **argv);
static int getWanGateway(int eid, webs_t wp, int argc, char_t **argv);
static int getRoutingTable(int eid, webs_t wp, int argc, char_t **argv);
static void setLan(webs_t wp, char_t *path, char_t *query);
static void setVpnPaThru(webs_t wp, char_t *path, char_t *query);
static void setWan(webs_t wp, char_t *path, char_t *query);
static void Operators(webs_t wp, char_t *path, char_t *query);
static void getMyMAC(webs_t wp, char_t *path, char_t *query);
static void addRouting(webs_t wp, char_t *path, char_t *query);
static void delRouting(webs_t wp, char_t *path, char_t *query);
static void dynamicRouting(webs_t wp, char_t *path, char_t *query);

static int get3gSignal(int eid, webs_t wp, int argc, char_t **argv);
static int getSimState(int eid, webs_t wp, int argc, char_t **argv);
static int get3gAttachState(int eid, webs_t wp, int argc, char_t **argv);
static int get3gESN(int eid, webs_t wp, int argc, char_t **argv);
static int get3gSN(int eid, webs_t wp, int argc, char_t **argv);
static int get3gPN(int eid, webs_t wp, int argc, char_t **argv);
static int get3gPIN(int eid, webs_t wp, int argc, char_t **argv);
static int getHrdVersion(int eid, webs_t wp, int argc, char_t **argv);
static int getDeviceID(int eid, webs_t wp, int argc, char_t **argv);

static int showOperatorsASP(int eid, webs_t wp, int argc, char_t **argv);
static int showNetworkTypeASP(int eid, webs_t wp, int argc, char_t **argv);
static int showMuanalBtnASP(int eid, webs_t wp, int argc, char_t **argv);

static void OperatorDelete(webs_t wp, char_t *path, char_t *query);

inline void zebraRestart(void);
void ripdRestart(void);

void formDefineInternet(void) {
	websAspDefine(T("getDhcpCliList"), getDhcpCliList);
	websAspDefine(T("getDns"), getDns);
	websAspDefine(T("getHostSupp"), getHostSupp);
	websAspDefine(T("getIfLiveWeb"), getIfLiveWeb);
	websAspDefine(T("getIfIsUpWeb"), getIfIsUpWeb);
	websAspDefine(T("getIgmpProxyBuilt"), getIgmpProxyBuilt);
	websAspDefine(T("getVPNBuilt"), getVPNBuilt);
	websAspDefine(T("getLanIp"), getLanIp);
	websAspDefine(T("getLanMac"), getLanMac);
	websAspDefine(T("getLanIfNameWeb"), getLanIfNameWeb);
	websAspDefine(T("getLanNetmask"), getLanNetmask);
	websAspDefine(T("getDnsmasqBuilt"), getDnsmasqBuilt);
	websAspDefine(T("getGWBuilt"), getGWBuilt);
	websAspDefine(T("getLltdBuilt"), getLltdBuilt);
	websAspDefine(T("getPppoeRelayBuilt"), getPppoeRelayBuilt);
	websAspDefine(T("getUpnpBuilt"), getUpnpBuilt);
	websAspDefine(T("getRadvdBuilt"), getRadvdBuilt);
	websAspDefine(T("getWanIp"), getWanIp);
	websAspDefine(T("getWanMac"), getWanMac);
	websAspDefine(T("getWanIfNameWeb"), getWanIfNameWeb);
	websAspDefine(T("getWanNetmask"), getWanNetmask);
	websAspDefine(T("getWanGateway"), getWanGateway);
	websAspDefine(T("getRoutingTable"), getRoutingTable);
	websAspDefine(T("getMeshBuilt"), getMeshBuilt);
	websAspDefine(T("getWDSBuilt"), getWDSBuilt);
	websAspDefine(T("getWSCBuilt"), getWSCBuilt);
	websAspDefine(T("getMBSSIDBuilt"), getMBSSIDBuilt);
	websAspDefine(T("getUSBBuilt"), getUSBBuilt);
	websAspDefine(T("getStorageBuilt"), getStorageBuilt);
	websAspDefine(T("getFtpBuilt"), getFtpBuilt);
	websAspDefine(T("getSmbBuilt"), getSmbBuilt);
	websAspDefine(T("getMediaBuilt"), getMediaBuilt);
	websAspDefine(T("getWebCamBuilt"), getWebCamBuilt);
	websAspDefine(T("getPrinterSrvBuilt"), getPrinterSrvBuilt);
	websAspDefine(T("getUSBiNICBuilt"), getUSBiNICBuilt);
	websFormDefine(T("setLan"), setLan);
	websFormDefine(T("setVpnPaThru"), setVpnPaThru);
	websFormDefine(T("setWan"), setWan);
    // add new
    websFormDefine(T("Operators"), Operators);
    websFormDefine(T("OperatorDelete"), OperatorDelete);    
    websAspDefine(T("showOperatorsASP"), showOperatorsASP);
	websAspDefine(T("showMuanalBtnASP"), showMuanalBtnASP);
	websAspDefine(T("showNetworkTypeASP"), showNetworkTypeASP);
    websAspDefine(T("get3gSignal"), get3gSignal);
	websAspDefine(T("getSimState"), getSimState);
    websAspDefine(T("get3gAttachState"), get3gAttachState);
    websAspDefine(T("get3gESN"), get3gESN);
    websAspDefine(T("get3gSN"), get3gSN);
    websAspDefine(T("get3gPN"), get3gPN);
    websAspDefine(T("get3gPIN"), get3gPIN);
    websAspDefine(T("getHrdVersion"), getHrdVersion);
    websAspDefine(T("getDeviceID"), getDeviceID);
    // add finish
	websFormDefine(T("getMyMAC"), getMyMAC);
	websFormDefine(T("addRouting"), addRouting);
	websFormDefine(T("delRouting"), delRouting);
	websFormDefine(T("dynamicRouting"), dynamicRouting);
	websAspDefine(T("getDynamicRoutingBuilt"), getDynamicRoutingBuilt);
	websAspDefine(T("getSWQoSBuilt"), getSWQoSBuilt);
	websAspDefine(T("getDATEBuilt"), getDATEBuilt);
	websAspDefine(T("getDDNSBuilt"), getDDNSBuilt);
	websAspDefine(T("getSysLogBuilt"), getSysLogBuilt);
	websAspDefine(T("getETHTOOLBuilt"), getETHTOOLBuilt);
}

/*
 * arguments: ifname  - interface name
 * description: test the existence of interface through /proc/net/dev
 * return: -1 = fopen error, 1 = not found, 0 = found
 */
int getIfLive(char *ifname)
{
	FILE *fp;
	char buf[256], *p;
	int i;

	if (NULL == (fp = fopen("/proc/net/dev", "r"))) {
		error(E_L, E_LOG, T("getIfLive: open /proc/net/dev error"));
		return -1;
	}

	fgets(buf, 256, fp);
	fgets(buf, 256, fp);
	while (NULL != fgets(buf, 256, fp)) {
		i = 0;
		while (isspace(buf[i++]))
			;
		p = buf + i - 1;
		while (':' != buf[i++])
			;
		buf[i-1] = '\0';
		if (!strcmp(p, ifname)) {
			fclose(fp);
			return 0;
		}
	}
	fclose(fp);
	error(E_L, E_LOG, T("getIfLive: device %s not found"), ifname);
	return 1;
}

/*
 * arguments: ifname  - interface name
 *            if_addr - a 18-byte buffer to store mac address
 * description: fetch mac address according to given interface name
 */
int getIfMac(char *ifname, char *if_hw)
{
	struct ifreq ifr;
	char *ptr;
	int skfd;

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error(E_L, E_LOG, T("getIfMac: open socket error"));
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
	if(ioctl(skfd, SIOCGIFHWADDR, &ifr) < 0) {
		close(skfd);
		//error(E_L, E_LOG, T("getIfMac: ioctl SIOCGIFHWADDR error for %s"), ifname);
		return -1;
	}

	ptr = (char *)&ifr.ifr_addr.sa_data;
	sprintf(if_hw, "%02X:%02X:%02X:%02X:%02X:%02X",
			(ptr[0] & 0377), (ptr[1] & 0377), (ptr[2] & 0377),
			(ptr[3] & 0377), (ptr[4] & 0377), (ptr[5] & 0377));

	close(skfd);
	return 0;
}

/*
 * arguments: ifname  - interface name
 *            if_addr - a 16-byte buffer to store ip address
 * description: fetch ip address, netmask associated to given interface name
 */
int getIfIp(char *ifname, char *if_addr)
{
	struct ifreq ifr;
	int skfd = 0;

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error(E_L, E_LOG, T("getIfIp: open socket error"));
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
	if (ioctl(skfd, SIOCGIFADDR, &ifr) < 0) {
		close(skfd);
		//error(E_L, E_LOG, T("getIfIp: ioctl SIOCGIFADDR error for %s"), ifname);
		return -1;
	}
	strcpy(if_addr, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

	close(skfd);
	return 0;
}

/*
 * arguments: ifname - interface name
 * description: return 1 if interface is up
 *              return 0 if interface is down
 */
int getIfIsUp(char *ifname)
{
	struct ifreq ifr;
	int skfd;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (skfd == -1) {
		perror("socket");
		return -1;
	}
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0) {
		perror("ioctl");
		close(skfd);
		return -1;
	}
	close(skfd);
	if (ifr.ifr_flags & IFF_UP)
		return 1;
	else
		return 0;
}

/*
 * arguments: ifname - interface name
 *            if_net - a 16-byte buffer to store subnet mask
 * description: fetch subnet mask associated to given interface name
 *              0 = bridge, 1 = gateway, 2 = wirelss isp
 */
int getIfNetmask(char *ifname, char *if_net)
{
	struct ifreq ifr;
	int skfd = 0;

	if((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		error(E_L, E_LOG, T("getIfNetmask: open socket error"));
		return -1;
	}

	strncpy(ifr.ifr_name, ifname, IF_NAMESIZE);
	if (ioctl(skfd, SIOCGIFNETMASK, &ifr) < 0) {
		close(skfd);
		//error(E_L, E_LOG, T("getIfNetmask: ioctl SIOCGIFNETMASK error for %s\n"), ifname);
		return -1;
	}
	strcpy(if_net, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	close(skfd);
	return 0;
}

/*
 * description: return WAN interface name
 *              0 = bridge, 1 = gateway, 2 = wirelss isp
 */
char* getWanIfName(void)
{
	char *mode = nvram_bufget(RT2860_NVRAM, "OperationMode");
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

char* getWanIfNamePPP(void)
{
    char *cm;
    cm = nvram_bufget(RT2860_NVRAM, "wanConnectionMode");
    if (!strncmp(cm, "PPPOE", 6) || !strncmp(cm, "L2TP", 5) || !strncmp(cm, "PPTP", 5) 
#ifdef CONFIG_USER_3G
		|| !strncmp(cm, "3G", 3)
#endif
	){
        return "ppp0";
	}

    return getWanIfName();
}


/*
 * description: return LAN interface name
 */
char* getLanIfName(void)
{
	char *mode = nvram_bufget(RT2860_NVRAM, "OperationMode");
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
		if(atoi(num_s) > 1)	// multiple ssid
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

/*
 * description: get the value "WAN" or "LAN" the interface is belong to.
 */
char *getLanWanNamebyIf(char *ifname)
{
	char *mode = nvram_bufget(RT2860_NVRAM, "OperationMode");

	if (NULL == mode)
		return "Unknown";

	if (!strcmp(mode, "0")){	// bridge mode
		if(!strcmp(ifname, "br0"))
			return "LAN";
		return ifname;
	}

	if (!strcmp(mode, "1")) {	// gateway mode
#if defined CONFIG_RAETH_ROUTER || defined CONFIG_MAC_TO_MAC_MODE || defined CONFIG_RT_3052_ESW
		if(!strcmp(ifname, "br0"))
			return "LAN";
		if(!strcmp(ifname, "eth2.2") || !strcmp(ifname, "ppp0"))
			return "WAN";
		return ifname;
#elif defined  CONFIG_ICPLUS_PHY && CONFIG_RT2860V2_AP_MBSS
		char *num_s = nvram_bufget(RT2860_NVRAM, "BssidNum");
		if(atoi(num_s) > 1 && !strcmp(ifname, "br0") )	// multiple ssid
			return "LAN";
		if(atoi(num_s) == 1 && !strcmp(ifname, "ra0"))
			return "LAN";
		if (!strcmp(ifname, "eth2") || !strcmp(ifname, "ppp0"))
			return "WAN";
		return ifname;
#else
		if(!strcmp(ifname, "ra0"))
			return "LAN";
		return ifname;
#endif
	}else if (!strncmp(mode, "2", 2)) {	// ethernet convertor
		if(!strcmp("eth2", ifname))
			return "LAN";
		if(!strcmp("ra0", ifname))
			return "WAN";
		return ifname;
	}else if (!strncmp(mode, "3", 2)) {	// apcli mode
		if(!strcmp("br0", ifname))
			return "LAN";
		if(!strcmp("apcli0", ifname))
			return "WAN";
		return ifname;
	}
	return ifname;
}


/*
 * description: write DHCP client list
 */
static int getDhcpCliList(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	struct dhcpOfferedAddr {
		unsigned char hostname[16];
		unsigned char mac[16];
		unsigned long ip;
		unsigned long expires;
	} lease;
	int i;
	struct in_addr addr;
	unsigned long expires;
	unsigned d, h, m;

	doSystem("killall -q -USR1 udhcpd");

	fp = fopen("/var/udhcpd.leases", "r");
	if (NULL == fp)
		return websWrite(wp, T(""));
	while (fread(&lease, 1, sizeof(lease), fp) == sizeof(lease)) {
		websWrite(wp, T("<tr><td>%-16s</td>"), lease.hostname);
		websWrite(wp, T("<td>%02X"), lease.mac[0]);
		for (i = 1; i < 6; i++)
			websWrite(wp, T(":%02X"), lease.mac[i]);
		addr.s_addr = lease.ip;
		expires = ntohl(lease.expires);
		websWrite(wp, T("</td><td>%s</td><td>"), inet_ntoa(addr));
		d = expires / (24*60*60); expires %= (24*60*60);
		h = expires / (60*60); expires %= (60*60);
		m = expires / 60; expires %= 60;
		if (d) websWrite(wp, T("%u days "), d);
		websWrite(wp, T("%02u:%02u:%02u\n"), h, m, (unsigned)expires);
	}
	fclose(fp);
	return 0;
}

/*

 * arguments: type - 1 = write Primary DNS
 *                   2 = write Secondary DNS
 * description: write DNS ip address accordingly
 */
static int getDns(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	char buf[80] = {0}, ns_str[11], dns[16] = {0};
	int type, idx = 0, req = 0;

	if (ejArgs(argc, argv, T("%d"), &type) == 1) {
		if (1 == type)
			req = 1;
		else if (2 == type)
			req = 2;
		else
			return websWrite(wp, T(""));
	}

	fp = fopen("/etc/resolv.conf", "r");
	if (NULL == fp)
		return websWrite(wp, T(""));
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (sscanf(buf, "%s%s", ns_str, dns) != 2)
			continue;
		idx++;
		if (idx == req) {
			websWrite(wp, T("%s"), dns);
			break;
		}
	}
	fclose(fp);
	return 0;
}

/*
 * arguments: 
 * description: return 1 if hostname is supported
 */
static int getHostSupp(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef GA_HOSTNAME_SUPPORT
	ejSetResult(eid, "1");
#else
	ejSetResult(eid, "0");
#endif
	return 0;
}

/*
 * arguments: name - interface name (ex. eth0, rax ..etc)
 * description: write the existence of given interface,
 *              0 = ifc dosen't exist, 1 = ifc exists
 */
static int getIfLiveWeb(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t *name;
	char exist[2] = "0";

	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	exist[0] = (getIfLive(name) == 0)? '1' : '0';
	return websWrite(wp, T("%s"), exist);
}

/*
 * arguments: name - interface name (ex. eth0, rax ..etc)
 * description: write the existence of given interface,
 *              0 = ifc is down, 1 = ifc is up
 */
static int getIfIsUpWeb(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t *name;
	char up[2] = "1";

	if (ejArgs(argc, argv, T("%s"), &name) < 1) {
		websError(wp, 400, T("Insufficient args\n"));
		return -1;
	}
	up[0] = (getIfIsUp(name) == 1)? '1' : '0';
	return websWrite(wp, T("%s"), up);
}

static int getIgmpProxyBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_USER_IGMP_PROXY
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getVPNBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_NF_CONNTRACK_PPTP || defined CONFIG_NF_CONNTRACK_PPTP_MODULE || \
    defined CONFIG_IP_NF_PPTP        || defined CONFIG_IP_NF_PPTP_MODULE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getMeshBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_RT2860V2_AP_MESH || defined CONFIG_RT2860V2_STA_MESH
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getWDSBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_RT2860V2_AP_WDS
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getWSCBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_RT2860V2_AP_WSC || defined CONFIG_RT2860V2_STA_WSC
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getMBSSIDBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_RT2860V2_AP_MBSS
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getUSBBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USB
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getStorageBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USB_STORAGE && defined CONFIG_USER_STORAGE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getFtpBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USER_STUPID_FTPD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getSmbBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USER_SAMBA
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getMediaBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USB && defined CONFIG_USER_USHARE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getWebCamBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USB && defined CONFIG_USER_UVC_STREAM
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getPrinterSrvBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USB && defined CONFIG_USER_P910ND
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getUSBiNICBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_INIC_USB 
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getDynamicRoutingBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USER_ZEBRA
    return websWrite(wp, T("1"));
#else
    return websWrite(wp, T("0"));
#endif
}

static int getSWQoSBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_RALINKAPP_SWQOS
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getDATEBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_DATE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getDDNSBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USER_INADYN
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getSysLogBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_LOGREAD && defined CONFIG_KLOGD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getETHTOOLBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USER_ETHTOOL
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

/*
 * description: write LAN ip address accordingly
 */
static int getLanIp(int eid, webs_t wp, int argc, char_t **argv)
{
	char if_addr[16];

	if (-1 == getIfIp(getLanIfName(), if_addr)) {
		//websError(wp, 500, T("getLanIp: calling getIfIp error\n"));
		return websWrite(wp, T(""));
	}
	return websWrite(wp, T("%s"), if_addr);
}

/*
 * description: write LAN MAC address accordingly
 */
static int getLanMac(int eid, webs_t wp, int argc, char_t **argv)
{
	char if_mac[18];

	if (-1 == getIfMac(getLanIfName(), if_mac)) {
		//websError(wp, 500, T("getLanIp: calling getIfMac error\n"));
		return websWrite(wp, T(""));
	}
	return websWrite(wp, T("%s"), if_mac);
}

/*
 * arguments: type - 0 = return LAN interface name (default)
 *                   1 = write LAN interface name
 * description: return or write LAN interface name accordingly
 */
static int getLanIfNameWeb(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char *name = getLanIfName();

	if (ejArgs(argc, argv, T("%d"), &type) == 1) {
		if (1 == type) {
			return websWrite(wp, T("%s"), name);
		}
	}
	ejSetResult(eid, name);
	return 0;
}

/*
 * description: write LAN subnet mask accordingly
 */
static int getLanNetmask(int eid, webs_t wp, int argc, char_t **argv)
{
	char if_net[16];

	if (-1 == getIfNetmask(getLanIfName(), if_net)) {
		//websError(wp, 500, T("getLanNetmask: calling getIfNetmask error\n"));
		return websWrite(wp, T(""));
	}
	return websWrite(wp, T("%s"), if_net);
}

static int getGWBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_LAN_WAN_SUPPORT || defined CONFIG_MAC_TO_MAC_MODE
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getDnsmasqBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_USER_DNSMASQ
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getLltdBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#if defined CONFIG_USER_LLTD && defined CONFIG_RT2860V2_AP_LLTD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getPppoeRelayBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_USER_RPPPPOE_RELAY
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getUpnpBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_USER_UPNP_IGD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

static int getRadvdBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_USER_RADVD
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}

/*
 * description: write WAN ip address accordingly
 */
static int getWanIp(int eid, webs_t wp, int argc, char_t **argv)
{
	char if_addr[16];

	if (-1 == getIfIp(getWanIfNamePPP(), if_addr)) {
		//websError(wp, 500, T("getWanIp: calling getIfIp error\n"));
		return websWrite(wp, T(""));
	}
	return websWrite(wp, T("%s"), if_addr);
}

/*
 * description: write WAN MAC address accordingly
 */
static int getWanMac(int eid, webs_t wp, int argc, char_t **argv)
{
	char if_mac[18];

	if (-1 == getIfMac(getWanIfName(), if_mac)) {
		//websError(wp, 500, T("getLanIp: calling getIfMac error\n"));
		return websWrite(wp, T("&nbsp;"));
	}
	return websWrite(wp, T("%s"), if_mac);
}

/*
 * arguments: type - 0 = return WAN interface name (default)
 *                   1 = write WAN interface name
 * description: return or write WAN interface name accordingly
 */
static int getWanIfNameWeb(int eid, webs_t wp, int argc, char_t **argv)
{
	int type;
	char *name = getWanIfName();

	if (ejArgs(argc, argv, T("%d"), &type) == 1) {
		if (1 == type) {
			return websWrite(wp, T("%s"), name);
		}
	}
	ejSetResult(eid, name);
	return 0;
}

/*
 * description: write WAN subnet mask accordingly
 */
static int getWanNetmask(int eid, webs_t wp, int argc, char_t **argv)
{
	char if_net[16];
	char *cm;

	cm = nvram_bufget(RT2860_NVRAM, "wanConnectionMode");
	if (!strncmp(cm, "PPPOE", 6) || !strncmp(cm, "L2TP", 5) || !strncmp(cm, "PPTP", 5) 
#ifdef CONFIG_USER_3G
			|| !strncmp(cm, "3G", 3)
#endif
	){ //fetch ip from ppp0
		if (-1 == getIfNetmask("ppp0", if_net)) {
			return websWrite(wp, T(""));
		}
	}
	else if (-1 == getIfNetmask(getWanIfName(), if_net)) {
		//websError(wp, 500, T("getWanNetmask: calling getIfNetmask error\n"));
		return websWrite(wp, T(""));
	}
	return websWrite(wp, T("%s"), if_net);
}

/*
 * description: write WAN default gateway accordingly
 */
static int getWanGateway(int eid, webs_t wp, int argc, char_t **argv)
{
	char   buff[256];
	int    nl = 0 ;
	struct in_addr dest;
	struct in_addr gw;
	int    flgs, ref, use, metric;
	unsigned long int d,g,m;
	int    find_default_flag = 0;

	char sgw[16];

	FILE *fp = fopen("/proc/net/route", "r");

	while (fgets(buff, sizeof(buff), fp) != NULL) {
		if (nl) {
			int ifl = 0;
			while (buff[ifl]!=' ' && buff[ifl]!='\t' && buff[ifl]!='\0')
				ifl++;
			buff[ifl]=0;    /* interface */
			if (sscanf(buff+ifl+1, "%lx%lx%X%d%d%d%lx",
						&d, &g, &flgs, &ref, &use, &metric, &m)!=7) {
				fclose(fp);
				return websWrite(wp, T("format error"));
			}

			if (flgs&RTF_UP) {
				dest.s_addr = d;
				gw.s_addr   = g;
				strcpy(sgw, (gw.s_addr==0 ? "" : inet_ntoa(gw)));

				if (dest.s_addr == 0) {
					find_default_flag = 1;
					break;
				}
			}
		}
		nl++;
	}
	fclose(fp);

	if (find_default_flag == 1)
		return websWrite(wp, T("%s"), sgw);
	else
		return websWrite(wp, T(""));
}

static char* get3gSignal_not_web(char *buf, int buflen);
static int get3gSignal(int eid, webs_t wp, int argc, char_t **argv)
{
	char   buff[256];
	char *pos;

	FILE *fp = fopen("/var/signal", "r");
	if(fp)
	{
		if (fgets(buff, sizeof(buff), fp) != NULL) {
			pos = strchr(buff, ',');
			if((pos != NULL) && (*pos == ','))
			{
				fclose(fp);
				sprintf(pos, ",(0-31)");
                if(strncmp(buff, "99", 2) == 0)
                    system("comgt -d /dev/yh -s /etc_ro/ppp/3g/signal.scr");
				return websWrite(wp, buff);
			}
		}
		fclose(fp);
	}
	
    return websWrite(wp, "No Signal");
    /*
    pos = get3gSignal_not_web(buff, sizeof(buff));
	if(pos != NULL)
	{
		printf("%s:%s\r\n", buff, pos);
		return websWrite(wp, buff);
	}
 
	return websWrite(wp, "No Signal");
	*/
}

static int getSimState(int eid, webs_t wp, int argc, char_t **argv)
{
	char   buff[256];
	char *pos;

	FILE *fp = fopen("/var/sim", "r");
	if(fp)
	{
		if (fgets(buff, sizeof(buff), fp) != NULL) {
			return websWrite(wp, buff);
		}
		fclose(fp);
	}
	return websWrite(wp, "Not Ready");
	/*
	pos = get3gSignal_not_web(buff, sizeof(buff));
	if(pos != NULL)
	{
		printf("%s:%s\r\n", buff, pos);
		return websWrite(wp, buff);
	}
 
	return websWrite(wp, "No Signal");
	*/
}


static int get3gAttachState(int eid, webs_t wp, int argc, char_t **argv)
{
	  char_t *pSubDev = nvram_bufget(RT2860_NVRAM, "wan_3g_dev");
	//char_t *nettype = websGetVar(wp, T("g3NetworkType"), T(""));
         char_t *nettype = nvram_bufget(RT2860_NVRAM, "g3_network_type");


	if(!pSubDev || !nettype)
	{
		return websWrite(wp, T("&nbsp;"));
	}

        if((strcmp(pSubDev,"SIERRA-MC8785")==0) ||(strcmp(pSubDev,"GTM681W")==0)){
                return websWrite(wp, T("AUTO"));
        }

	if((strcmp(pSubDev, "HUAWEI-EM660") == 0)||(strcmp(pSubDev, "IE901D") == 0)||(strcmp(pSubDev,"THINKWILL-MI600")==0)||(strcmp(pSubDev,"SYNCWISER-801/401")==0)||(strcmp(pSubDev,"LONGSUNG-C5300")==0)||(strcmp(pSubDev,"KSE-360")==0))
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("CDMA/EVDO HYBRID"));

			case 1:
				return websWrite(wp, T("EVDO Only"));

			case 2:
				return websWrite(wp, T("CDMA Only"));
			
			default:
				return websWrite(wp, T("&nbsp;"));
		}
	}
	else if(strcmp(pSubDev, "HUAWEI-EM770") == 0)
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("Automatic search"));

			case 1:
				return websWrite(wp, T("WCDMA ONLY"));

			case 2:
				return websWrite(wp, T("GSM ONLY"));
			
			case 3:
				return websWrite(wp, T("No change"));

			default:
				return websWrite(wp, T("&nbsp;"));
		}
	}
	else if(strcmp(pSubDev, "HUAWEI-EM560") == 0)
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("Automatic search"));

			case 1:
				return websWrite(wp, T("TDSCDMA ONLY"));

			case 2:
				return websWrite(wp, T("WCDMA ONLY"));
			
			case 3:
				return websWrite(wp, T("GSM ONLY"));

			default:
				return websWrite(wp, T("&nbsp;"));
		}
	}
	else if(strcmp(pSubDev, "LONGSUNG-U6300/U5300") == 0)
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("WCDMA PREFERRED"));

			case 1:
				return websWrite(wp, T("WCDMA ONLY"));

			case 2:
				return websWrite(wp, T("GSM PREFERRED"));
			
			case 3:
				return websWrite(wp, T("GSM ONLY"));

			default:
				return websWrite(wp, T("&nbsp;"));
		}
	}
	else if(strcmp(pSubDev, "GAORAN-280") == 0){
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("Automatic search"));

			case 1:
				return websWrite(wp, T("WCDMA ONLY"));

			case 2:
				return websWrite(wp, T("GSM ONLY"));
			default:
				return websWrite(wp, T("&nbsp;"));
		}

	}
	else if(strcmp(pSubDev, "TW-W1M100") == 0)
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("WCDMA PREFERRED"));

			case 1:
				return websWrite(wp, T("WCDMA ONLY"));

			case 2:
				return websWrite(wp, T("GSM PREFERRED"));
			
			case 3:
				return websWrite(wp, T("GSM ONLY"));

			default:
				return websWrite(wp, T("&nbsp;"));
		}
	}
	else if(strcmp(pSubDev, "MC5728") == 0)
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("CDMA/EVDO HYBRID"));
		}
	}
	else if(strcmp(pSubDev, "SIMCOM-SIM700") == 0)
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("GSM/EDGE"));
		}
	}
	else if(strcmp(pSubDev, "ZTE-MU301") == 0)
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("Automatic search"));

			case 1:
				return websWrite(wp, T("TDSCDMA ONLY"));

			case 2:
				return websWrite(wp, T("WCDMA ONLY"));
			
			case 3:
				return websWrite(wp, T("GSM ONLY"));
			default:
				return websWrite(wp, T("&nbsp;"));
		}
	}
	else if(strcmp(pSubDev, "ZTE-MF210V") == 0){
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("Automatic search"));

			case 1:
				return websWrite(wp, T("WCDMA ONLY"));

			case 2:
				return websWrite(wp, T("GSM ONLY"));
			default:
				return websWrite(wp, T("&nbsp;"));
		}

	}
	else if(strcmp(pSubDev, "ZX-600") == 0)
	{
		switch(atoi(nettype))
		{
			case 0:
				return websWrite(wp, T("AUTO MODE"));
			case 1:
				return websWrite(wp, T("WCDMA PREFERRED"));

			case 2:
				return websWrite(wp, T("WCDMA ONLY"));

			case 3:
				return websWrite(wp, T("GSM PREFERRED"));
			
			case 4:
				return websWrite(wp, T("GSM ONLY"));

			default:
				return websWrite(wp, T("&nbsp;"));
		}
	}

	else
		return websWrite(wp, T("&nbsp;"));
}

static int get3gESN(int eid, webs_t wp, int argc, char_t **argv)
{
    FILE   *stream;
	char   buf[128];

    char *pdev = nvram_bufget(RT2860_NVRAM, "wan_3g_dev"); 
    memset( buf, '\0', sizeof(buf) );//初始化buf,以免后面写如乱码到文件中

	if(strcmp(pdev,"HUAWEI-EM660")==0)
	{
    //stream = popen( "comgt -d /dev/ttyUSB2 -s /etc_ro/ppp/3g/esn.scr", "r" ); //将�?�ls －l”命令的输出 通过管道读取（�?�r”参数）到FILE* stream

	if(strcmp(pdev, "HUAWEI-EM560") == 0)
	{
//		stream=popen("3GInfo -d /dev/ttyACM2 -e","r");
		stream=popen("comgt -d /dev/ttyACM2 -s /etc_ro/ppp/3g/esn.scr","r");
	}
	else
	{
		//stream=popen("3GInfo -d /dev/ttyUSB2 -e","r");
		stream=popen("comgt -d /dev/ttyUSB2 -s /etc_ro/ppp/3g/esn.scr","r");
	}
    fread( buf, sizeof(char), sizeof(buf), stream); //将刚刚FILE* stream的数据流读取到buf�?   

	 pclose( stream );  

	}
		
	if((buf[0] == NULL)||(strlen(buf)>20))
	{
		strcpy(buf, T("&nbsp;"));
	}

    return websWrite(wp, buf);
}

static int get3gSN(int eid, webs_t wp, int argc, char_t **argv)
{
    return websWrite(wp, T("&nbsp;"));
}

static int get3gPN(int eid, webs_t wp, int argc, char_t **argv)
{
    return websWrite(wp, T("&nbsp;"));
}

static int get3gPIN(int eid, webs_t wp, int argc, char_t **argv)
{
    return websWrite(wp, T("&nbsp;"));
}

static int getHrdVersion(int eid, webs_t wp, int argc, char_t **argv)
{
    return websWrite(wp, T("H 1.0"));
}

static int getDeviceID(int eid, webs_t wp, int argc, char_t **argv)
{

    char   buff[256];
    char data[256];
	char *pos;
	int startFlag = 0;
    int i = 0;
    int index = 0;
	FILE *fp = fopen("/var/imei", "r");
		if(fp)
		{
			if (fgets(buff, sizeof(buff), fp) != NULL) 
            {
                for(i = 0; i < sizeof(buff); i++)
                {
                    if((buff[i] == 0x0d) || (buff[i] == 0x0a) || (buff[i] == 0x00))
                    {
                        break;
                    }
                }
                buff[i] = 0;
				 for(i = 0;i < sizeof(buff);i++)
                 {
                        if(startFlag == 0)
                        {
                            if(buff[i] == ':') 
                                 startFlag = 1;//find the start
                        }
                        else
                            data[index++] = buff[i];
                }
                
                if(startFlag == 1)
                    return websWrite(wp, data);
                else
                    return websWrite(wp, buff);
			}
            else
            {
             
			    fclose(fp);
                return websWrite(wp, "no file");
            }
		}
		
		return websWrite(wp, "error");

	
	 
	  /*  char buf[20];
    char data[20];
    int startFlag = 0;
    int i = 0;
    int index = 0;

	FILE *fp = fopen("/var/imei", "r");
	memset(buf,0,sizeof(buf));
	while(fgets(buf, sizeof(buf), fp) != NULL)
	{
 
		if(!isspace(buf[0]))
		{
			break;
		} 
	}
	if(fp) 
		pclose(fp);

    for(i = 0;i < 20;i++)
    {
        if(startFlag == 0)
        {
            if(buf[i] == ':') 
                 startFlag = 1;//afind the start
        }
        else
            data[index++] = buf[i];
    }
    return websWrite(wp, data);*/
}

#define DD printf("%d\n", __LINE__);fflush(stdout);

/*
 *
 */
int getIndexOfRoutingRule(char *dest, char *netmask, char *interface)
{
	int index=0;
	char *rrs, one_rule[256];
	char dest_f[32], netmask_f[32], interface_f[32];
	rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules");
	if(!rrs || !strlen(rrs))
		return -1;

	while( getNthValueSafe(index, rrs, ';', one_rule, 256) != -1 ){
		if((getNthValueSafe(0, one_rule, ',', dest_f, sizeof(dest_f)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(1, one_rule, ',', netmask_f, sizeof(netmask_f)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(4, one_rule, ',', interface_f, sizeof(interface_f)) == -1)){
			index++;
			continue;
		}
		//printf("@@@@@ %s %s %s\n", dest_f, netmask_f, interface_f);
		//printf("----- %s %s %s\n", dest, netmask, interface);
		if( (!strcmp(dest, dest_f)) && (!strcmp(netmask, netmask_f)) && (!strcmp(interface, interface_f))){
			return index;
		}
		index++;
	}

	return -1;
}

static void removeRoutingRule(char *dest, char *netmask, char *ifname)
{
	char cmd[1024];
	strcpy(cmd, "route del ");
	
	// host or net?
	if(!strcmp(netmask, "255.255.255.255") )
		strcat(cmd, "-host ");
	else
		strcat(cmd, "-net ");

	// destination
	strcat(cmd, dest);
	strcat(cmd, " ");

	// netmask
	if(strcmp(netmask, "255.255.255.255"))
		sprintf(cmd, "%s netmask %s", cmd, netmask);

	//interface
	sprintf(cmd, "%s dev %s ", cmd, ifname);
	doSystem(cmd);
}

void staticRoutingInit(void)
{
	int index=0;
	char one_rule[256];
	char *rrs;
	struct in_addr dest_s, gw_s, netmask_s;
	char dest[32], netmask[32], gw[32], interface[32], true_interface[32], custom_interface[32], comment[32];
	int	flgs, ref, use, metric, nl=0;
	unsigned long int d,g,m;
	int isGatewayMode = (!strcmp("1", nvram_bufget(RT2860_NVRAM, "OperationMode"))) ? 1 : 0 ;

	// delete old user rules
	FILE *fp = fopen("/proc/net/route", "r");
	if(!fp)
		return;

	while (fgets(one_rule, sizeof(one_rule), fp) != NULL) {
		if (nl) {
			if (sscanf(one_rule, "%s%lx%lx%X%d%d%d%lx",
					interface, &d, &g, &flgs, &ref, &use, &metric, &m) != 8) {
				printf("format error\n");
				fclose(fp);
				return;
			}
			dest_s.s_addr = d;
			gw_s.s_addr = g;
			netmask_s.s_addr = m;

			strncpy(dest, inet_ntoa(dest_s), sizeof(dest));
			strncpy(gw, inet_ntoa(gw_s), sizeof(gw));
			strncpy(netmask, inet_ntoa(netmask_s), sizeof(netmask));

			// check if internal routing rules
			if( (index=getIndexOfRoutingRule(dest, netmask, interface)) != -1){
				removeRoutingRule(dest, netmask, interface);
			}
		}
		nl++;
	}
	fclose(fp);

	index = 0;
	rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules");
	if(!rrs|| !strlen(rrs))
		return;

	while( getNthValueSafe(index, rrs, ';', one_rule, 256) != -1 ){
		char cmd[1024];

		if((getNthValueSafe(0, one_rule, ',', dest, sizeof(dest)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(1, one_rule, ',', netmask, sizeof(netmask)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(2, one_rule, ',', gw, sizeof(gw)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(3, one_rule, ',', interface, sizeof(interface)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(4, one_rule, ',', true_interface, sizeof(true_interface)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(5, one_rule, ',', custom_interface, sizeof(custom_interface)) == -1)){
			index++;
			continue;
		}
		if((getNthValueSafe(6, one_rule, ',', comment, sizeof(comment)) == -1)){
			index++;
			continue;
		}

		strcpy(cmd, "route add ");
		
		// host or net?
		if(!strcmp(netmask, "255.255.255.255") )
			strcat(cmd, "-host ");
		else
			strcat(cmd, "-net ");

		// destination
		strcat(cmd, dest);
		strcat(cmd, " ");

		// netmask
		if(strcmp(netmask, "255.255.255.255") )
			sprintf(cmd, "%s netmask %s", cmd, netmask);

		// gateway
		if(strcmp(gw, "0.0.0.0"))
			sprintf(cmd, "%s gw %s", cmd, gw);

		//interface
//		if (!strcmp(interface, "WAN")){
//			true_interface = getWanIfName();
//		}else if (!gstrcmp(interface, "Custom")){
//			true_interface = custom_interface;
//		}else	// LAN & unknown
//			true_interface = getLanIfName();

		sprintf(cmd, "%s dev %s ", cmd, true_interface);

		strcat(cmd, "2>&1 ");

		if(strcmp(interface, "WAN") || (!strcmp(interface, "WAN") && isGatewayMode)  ){
			doSystem(cmd);
		}else{
			printf("Skip WAN routing rule in the non-Gateway mode: %s\n", cmd);
		}

		index++;
	}
	return;
}

void dynamicRoutingInit(void)
{
	zebraRestart();
	ripdRestart();
}

void RoutingInit(void)
{
	staticRoutingInit();
	dynamicRoutingInit();
}

static inline int getNums(char *value, char delimit)
{
    char *pos = value;
    int count=1;
    if(!pos || !strlen(pos))
        return 0;
    while( (pos = strchr(pos, delimit))){
        pos = pos+1;
        count++;
    }
    return count;
}

/*
 * description: get routing table
 */
static int getRoutingTable(int eid, webs_t wp, int argc, char_t **argv)
{
	char   result[4096] = {0};
	char   buff[512];
	int    nl = 0, index;
	char   ifname[32], interface[128];
	struct in_addr dest, gw, netmask;
	char   dest_str[32], gw_str[32], netmask_str[32], comment[32];
	int    flgs, ref, use, metric;
	int	   *running_rules = NULL;
	unsigned long int d,g,m;
	char *rrs;
	int  rule_count;
	FILE *fp = fopen("/proc/net/route", "r");
	if(!fp)
		return -1;

	rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules");
	rule_count = getNums(rrs, ';');

	if(rule_count){
		running_rules = calloc(1, sizeof(int) * rule_count);
		if(!running_rules)
			return -1;
	}
		
	strncat(result, "\"", sizeof(result));
	while (fgets(buff, sizeof(buff), fp) != NULL) {
		if (nl) {
			if (sscanf(buff, "%s%lx%lx%X%d%d%d%lx",
					ifname, &d, &g, &flgs, &ref, &use, &metric, &m) != 8) {
				printf("format error\n");
				fclose(fp);
				return websWrite(wp, T(""));
			}
			dest.s_addr = d;
			gw.s_addr = g;
			netmask.s_addr = m;

			if(! (flgs & 0x1) )	// skip not usable
				continue;

			strncpy(dest_str, inet_ntoa(dest), sizeof(dest_str));
			strncpy(gw_str, inet_ntoa(gw), sizeof(gw_str));
			strncpy(netmask_str, inet_ntoa(netmask), sizeof(netmask_str));

			if(nl > 1)
				strncat(result, ";", sizeof(result));
			strncat(result, ifname, sizeof(result));		strncat(result, ",", sizeof(result));
			strncat(result, dest_str, sizeof(result));		strncat(result, ",", sizeof(result));
			strncat(result, gw_str, sizeof(result));			strncat(result, ",", sizeof(result));
			strncat(result, netmask_str, sizeof(result) );	strncat(result, ",", sizeof(result));
			snprintf(result, sizeof(result), "%s%d,%d,%d,%d,", result, flgs, ref, use, metric);

			// check if internal routing rules
			strcpy(comment, " ");
			if( (index=getIndexOfRoutingRule(dest_str, netmask_str, ifname)) != -1){
				char one_rule[256];

				if(index < rule_count)
					running_rules[index] = 1;
				else
					printf("fatal error in %s\n", __FUNCTION__);

				snprintf(result, sizeof(result), "%s%d,", result, index);
				if(rrs && strlen(rrs)){
					if( getNthValueSafe(index, rrs, ';', one_rule, sizeof(one_rule)) != -1){

						if( getNthValueSafe(3, one_rule, ',', interface, sizeof(interface)) != -1){
							strncat(result, interface, sizeof(result));
							strncat(result, ",", sizeof(result));
						}
						if( getNthValueSafe(6, one_rule, ',', comment, sizeof(comment)) != -1){
							// do nothing;
						}
					}
				}
			}else{
				strncat(result, "-1,", sizeof(result));
				strncat(result, getLanWanNamebyIf(ifname), sizeof(result));
				strncat(result, ",", sizeof(result));
			}
			strncat(result, "0,", sizeof(result));	// used rule
			strncat(result, comment, sizeof(result));
		}
		nl++;
	}

	for(index=0; index < rule_count; index++){
		char one_rule[256];

		if(running_rules[index])
			continue;

		if(getNthValueSafe(index, rrs, ';', one_rule, sizeof(one_rule)) == -1)
			continue;

		if(getNthValueSafe(0, one_rule, ',', dest_str, sizeof(dest_str)) == -1)
			continue;

		if(getNthValueSafe(1, one_rule, ',', netmask_str, sizeof(netmask_str)) == -1)
			continue;

		if(getNthValueSafe(2, one_rule, ',', gw_str, sizeof(gw_str)) == -1)
			continue;

		if(getNthValueSafe(3, one_rule, ',', interface, sizeof(interface)) == -1)
			continue;

		if(getNthValueSafe(4, one_rule, ',', ifname, sizeof(ifname)) == -1)
			continue;

		if(getNthValueSafe(6, one_rule, ',', comment, sizeof(comment)) == -1)
			continue;

		if(strlen(result))
			strncat(result, ";", sizeof(result));

		snprintf(result, sizeof(result), "%s%s,%s,%s,%s,0,0,0,0,%d,%s,1,%s", result, ifname, dest_str, gw_str, netmask_str, index, interface, comment);
	}

	strcat(result, "\"");
	websLongWrite(wp, result);
	fclose(fp);
	if(running_rules)
		free(running_rules);
	//printf("%s\n", result);
	return 0;
}

static void addRouting(webs_t wp, char_t *path, char_t *query)
{
	char_t *dest, *hostnet, *netmask, *gateway, *interface, *true_interface, *custom_interface, *comment;
	char cmd[256] = {0};
	char result[256] = {0};

	FILE *fp;

	dest = websGetVar(wp, T("dest"), T(""));
	hostnet = websGetVar(wp, T("hostnet"), T(""));
	netmask = websGetVar(wp, T("netmask"), T(""));	
	gateway = websGetVar(wp, T("gateway"), T(""));
	interface = websGetVar(wp, T("interface"), T(""));
	custom_interface = websGetVar(wp, T("custom_interface"), T(""));
	comment = websGetVar(wp, T("comment"), T(""));

	if( !dest)
		return;

	strcat(cmd, "route add ");
	
	// host or net?
	if(!gstrcmp(hostnet, "net"))
		strcat(cmd, "-net ");
	else
		strcat(cmd, "-host ");

	// destination
	strcat(cmd, dest);
	strcat(cmd, " ");

	// netmask
	if(gstrlen(netmask))
		sprintf(cmd, "%s netmask %s", cmd, netmask);
	else
		netmask = "255.255.255.255";

	//gateway
	if(gstrlen(gateway))
		sprintf(cmd, "%s gw %s", cmd, gateway);
	else
		gateway = "0.0.0.0";

	//interface
	if(gstrlen(interface)){
		if (!gstrcmp(interface, "WAN")){
			true_interface = getWanIfName();
		}else if (!gstrcmp(interface, "Custom")){
			if(!gstrlen(custom_interface))
				return;
			true_interface = custom_interface;
		}else	// LAN & unknown
			true_interface = getLanIfName();
	}else{
		interface = "LAN";
		true_interface = getLanIfName();
	}
	sprintf(cmd, "%s dev %s ", cmd, true_interface);

	strcat(cmd, "2>&1 ");

	printf("%s\n", cmd);
	fp = popen(cmd, "r");
	fgets(result, sizeof(result), fp);
	pclose(fp);


	if(!strlen(result)){
		// success, write down to the flash
		char tmp[1024];
		char *rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules");
		if(!rrs || !strlen(rrs)){
			memset(tmp, 0, sizeof(tmp));
		}else{
			strncpy(tmp, rrs, sizeof(tmp));
		}
		if(strlen(tmp))
			strcat(tmp, ";");
		sprintf(tmp, "%s%s,%s,%s,%s,%s,%s,%s", tmp, dest, netmask, gateway, interface, true_interface, custom_interface, comment);
		nvram_bufset(RT2860_NVRAM, "RoutingRules", tmp);
		nvram_commit(RT2860_NVRAM);
	}else{
		websHeader(wp);		
		websWrite(wp, T("<h1>Add routing failed:<br> %s<h1>"), result);
		websFooter(wp);
		websDone(wp, 200);
		return;
	}

	//debug print
	websHeader(wp);
	websWrite(wp, T("<h3>Add routing table:</h3><br>\n"));
	if(strlen(result)){
		websWrite(wp, T("Success"));
	}else
		websWrite(wp, T("%s"), result);

	websWrite(wp, T("Destination: %s<br>\n"), dest);
	websWrite(wp, T("Host/Net: %s<br>\n"), hostnet);
	websWrite(wp, T("Netmask: %s<br>\n"), netmask);
	websWrite(wp, T("Gateway: %s<br>\n"), gateway);
	websWrite(wp, T("Interface: %s<br>\n"), interface);
	websWrite(wp, T("True Interface: %s<br>\n"), true_interface);
	if(strlen(custom_interface))
		websWrite(wp, T("Custom_interface %s<br>\n"), custom_interface);
	websWrite(wp, T("Comment: %s<br>\n"), comment);
	websFooter(wp);
	websDone(wp, 200);
}

static void delRouting(webs_t wp, char_t *path, char_t *query)
{
	int index, rule_count;
	char_t *value, dest[256], netmask[256], true_interface[256];
	char name_buf[16] = {0};
	char *rrs;
	int *deleArray, j=0;
	
	rrs = nvram_bufget(RT2860_NVRAM, "RoutingRules");
	if(!rrs || !strlen(rrs))
		return;

	rule_count = getNums(rrs, ';');
	if(!rule_count)
		return;

	if(!(deleArray = malloc(sizeof(int) * rule_count) ) )
		return;
		
	websHeader(wp);

	for(index=0; index< rule_count; index++){
		snprintf(name_buf, sizeof(name_buf), "DR%d", index);
		value = websGetVar(wp, name_buf, NULL);
		if(value){
			deleArray[j++] = index;
			if(strlen(value) > 256)
				continue;
			sscanf(value, "%s%s%s", dest, netmask, true_interface);
			removeRoutingRule(dest, netmask, true_interface);
			websWrite(wp, T("Delete entry: %s,%s,%s<br>\n"), dest, netmask, true_interface);
		}
	}

	if(j>0){
		deleteNthValueMulti(deleArray, j, rrs, ';');
		nvram_bufset(RT2860_NVRAM, "RoutingRules", rrs);
		nvram_commit(RT2860_NVRAM);
	}

	websFooter(wp);
	websDone(wp, 200);

	free(deleArray);
}

void ripdRestart(void)
{
	char lan_ip[16], wan_ip[16], lan_mask[16], wan_mask[16];

	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *password = nvram_bufget(RT2860_NVRAM, "Password");
	char *RIPEnable = nvram_bufget(RT2860_NVRAM, "RIPEnable");

	doSystem("killall -q ripd");

	if(!opmode||!strlen(opmode))
		return;
	if(!strcmp(opmode, "0"))	// bridge
		return;

	if(!RIPEnable || !strlen(RIPEnable) || !strcmp(RIPEnable,"0"))
        return;

	if(!password || !strlen(password))
		password = "rt2880";

	doSystem("echo \"hostname linux.router1\" > /etc/ripd.conf ");
	doSystem("echo \"password %s\" >> /etc/ripd.conf ", password);
	doSystem("echo \"router rip\" >> /etc/ripd.conf ");

	// deal with WAN
	if(getIfIp(getWanIfName(), wan_ip) != -1){
		if(getIfNetmask(getWanIfName(), wan_mask) != -1){
			doSystem("echo \"network %s/%d\" >> /etc/ripd.conf", wan_ip, netmask_aton(wan_mask));
			doSystem("echo \"network %s\" >> /etc/ripd.conf", getWanIfName());
		}else
			printf("ripdRestart(): The WAN IP is still undeterminated...\n");
	}else
		printf("ripdRestart(): The WAN IP is still undeterminated...\n");

	// deal with LAN
	if(getIfIp(getLanIfName(), lan_ip) != -1){
		if(getIfNetmask(getLanIfName(), lan_mask) != -1){
			doSystem("echo \"network %s/%d\" >> /etc/ripd.conf", lan_ip, netmask_aton(lan_mask));
			doSystem("echo \"network %s\" >> /etc/ripd.conf", getLanIfName());
		}
	}
	doSystem("echo \"version 2\" >> /etc/ripd.conf");
	doSystem("echo \"log syslog\" >> /etc/ripd.conf");
	doSystem("ripd -f /etc/ripd.conf -d");
}

inline void zebraRestart(void)
{
	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *password = nvram_bufget(RT2860_NVRAM, "Password");

	char *RIPEnable = nvram_bufget(RT2860_NVRAM, "RIPEnable");

	doSystem("killall -q zebra");

	if(!opmode||!strlen(opmode))
		return;
	if(!strcmp(opmode, "0"))	// bridge
		return;

	if(!RIPEnable || !strlen(RIPEnable) || !strcmp(RIPEnable,"0"))
		return;

	if(!password || !strlen(password))
		password = "rt2880";

	doSystem("echo \"hostname linux.router1\" > /etc/zebra.conf ");
	doSystem("echo \"password %s\" >> /etc/zebra.conf ", password);
	doSystem("echo \"enable password rt2880\" >> /etc/zebra.conf ");
	doSystem("echo \"log syslog\" >> /etc/zebra.conf ");
	doSystem("zebra -d -f /etc/zebra.conf");
}

static void dynamicRouting(webs_t wp, char_t *path, char_t *query)
{
	char_t *rip;
	char *RIPEnable;
	rip = websGetVar(wp, T("RIPSelect"), T(""));
	if(!rip || !strlen(rip))
		return;

	RIPEnable = nvram_bufget(RT2860_NVRAM, "RIPEnable");
	if(!RIPEnable || !strlen(RIPEnable))
		RIPEnable = "0";

	if(!gstrcmp(rip, "0") && !strcmp(RIPEnable, "0")){
		// nothing changed
	}else if(!gstrcmp(rip, "1") && !strcmp(RIPEnable, "1")){
		// nothing changed
	}else if(!gstrcmp(rip, "0") && !strcmp(RIPEnable, "1")){
		nvram_bufset(RT2860_NVRAM, "RIPEnable", rip);
		nvram_commit(RT2860_NVRAM);
		doSystem("killall -q ripd");
		doSystem("killall -q zebra");
	}else if(!gstrcmp(rip, "1") && !strcmp(RIPEnable, "0")){
		nvram_bufset(RT2860_NVRAM, "RIPEnable", rip);
		nvram_commit(RT2860_NVRAM);
		zebraRestart();
		ripdRestart();
	}else{
		return;
	}

	//debug print
	websHeader(wp);
	websWrite(wp, T("<h3>Dynamic Routing:</h3><br>\n"));
	websWrite(wp, T("RIPEnable %s<br>\n"), rip);
	websFooter(wp);
	websDone(wp, 200);
}

static char* get3gSignal_not_web(char *buf, int buflen)
{
	FILE   *stream;
	char*pdev;

    pdev = nvram_bufget(RT2860_NVRAM, "wan_3g_dev"); 
    memset( buf, '\0', buflen );//初始化buf,以免后面写如乱码到文件中
 //   stream = popen( "comgt -d /dev/ttyUSB2 -s /etc_ro/ppp/3g/signal.scr", "r" ); //将�?�ls －l”命令的输出 通过管道读取（�?�r”参数）到FILE* stream


	if((strcmp(pdev,"MC5728")==0)||(strcmp(pdev,"SIMCOM-SIM700")==0)||(strcmp(pdev,"GTM681W")==0)){
		return NULL;
	}

	if(strcmp(pdev, "HUAWEI-EM560") == 0)
	{
	//	stream=popen("3GInfo -d /dev/ttyACM2 -s","r");
		//stream=popen("comgt -d /dev/ttyACM2 -s /etc_ro/ppp/3g/signal.scr","r");
		stream=popen("3GInfo -d /dev/ttyACM2 ","r");
	}
	else if(strcmp(pdev, "HUAWEI-EM660") == 0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB2 -m 0-31","r");
	}
	else if(strcmp(pdev, "IE901D") == 0)
        {
                stream=popen("3GInfo -d /dev/ttyUSB1 -m 0-31","r");
        }

	else if(strcmp(pdev, "HUAWEI-EM770") == 0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB2 -m 0-31","r");
	}
	else if(strcmp(pdev,"THINKWILL-MI600")==0)
	{
		//stream=popen("comgt -d /dev/ttyUSB4 -s /etc_ro/ppp/3g/signal.scr","r");
		stream=popen("3GInfo -d /dev/ttyUSB4 -m 0-31","r"); //-m signal range
	}
	else if(strcmp(pdev,"SYNCWISER-801/401")==0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB2 -c AT*ST*INFO? -m 1-5 ","r"); //-m signal range
	}
	else if(strcmp(pdev,"LONGSUNG-C5300")==0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB3  -m 0-31 ","r"); //-m signal range
	}
	else if(strcmp(pdev,"LONGSUNG-U6300/U5300")==0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB1  -m 0-31 ","r"); //-m signal range
	}
	else if(strcmp(pdev,"GAORAN-280")==0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB3  -m 0-31 ","r"); //-m signal range
	}
	else if(strcmp(pdev,"TW-W1M100")==0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB1  -m 0-31 ","r"); //-m signal range
	}
	else if(strcmp(pdev,"ZTE-MU301")==0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB2","r"); //-m signal range
	}
	else if(strcmp(pdev,"ZTE-MF210V")==0)
	{
		//stream=popen("3GInfo -d /dev/ttyUSB2  -m 0-31 ","r"); //-m signal range MF210V
		stream=popen("3GInfo -d /dev/ttyUSB1  -m 0-31 ","r"); //-m signal range  MF210
	}
	else if(strcmp(pdev,"KSE-360")==0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB1 -m 0-31 ","r"); //-m signal range
	}
	else if(strcmp(pdev,"ZX-600")==0)
	{
		stream=popen("3GInfo -d /dev/ttyUSB2 -m 0-31 ","r"); //-m signal range
	}
	
         else if(strcmp(pdev,"SIERRA-MC8785")==0)
        {
                stream=popen("3GInfo -d /dev/ttyUSB3 -m 0-31 ","r"); //-m signal range

        }

	else 
	{
		stream=popen("3GInfo -d /dev/ttyUSB2","r");
	}

    fread( buf, sizeof(char), sizeof(buf), stream); //将刚刚FILE* stream的数据流读取到buf�?  
	  pclose( stream );  
	printf("%s\r\n", buf);
	if((buf[0] < '0') || (buf[0] > '9'))
	{
		return NULL;
	}

/*	
//	if((buf[0] == NULL)||(strlen(buf)>7))
	if((buf[0] == NULL)||(strlen(buf)>7))
	{
		strcpy(buf, T("&nbsp;"));
	}
*/
    return buf;
}
/*
 * description: setup internet according to nvram configurations
 *              (assume that nvram_init has already been called)
 *              return value: 0 = successful, -1 = failed
 */
int initInternet(void)
{
#ifndef CONFIG_RALINK_RT2880
	char *auth_mode = nvram_bufget(RT2860_NVRAM, "AuthMode");
#endif
#if defined CONFIG_RT2860V2_STA || defined CONFIG_RT2860V2_STA_MODULE
	char *opmode;
#endif
	char   buf[128];
/*
	int times = 3;
	while(times--)
	{
		FILE   *stream;
		

		memset(buf, 0, sizeof(buf));
		stream=popen("comgt -d /dev/yh -s /etc_ro/ppp/3g/sim.scr","r");
		if(stream)
		{
			fread( buf, sizeof(char), sizeof(buf), stream); 
			printf("Get result when ask for sim card status:%s\r\n", buf);
			pclose( stream ); 
		}

		if((buf[0] != NULL)&&(!strcmp(buf, "Inserted")))
		{
			break;
		}
	}

	if(times == 0)
	{
		strcpy(wan_3g_sim, "Not Ready");
		strcpy(wan_3g_signal, "No Signal");
		//nvram_bufset(RT2860_NVRAM, "wan_3g_signal", "No Signal");
		printf("no sim card inserted\r\n");
	}
	else
	{
		char *psignal = NULL;
		strcpy(wan_3g_sim, "Inserted");
		times = 3;
		while(times--)
		{
			psignal = get3gSignal_not_web(buf, sizeof(buf));
		}

		if(!psignal)
		{
			printf("no signal!!!!!\r\n");
			strcpy(wan_3g_signal, "No signal");
			//return -1;
		}
		else
		{
			strcpy(wan_3g_signal, buf);
		}
	}
*/
	doSystem("internet.sh");

	//automatically connect to AP according to the active profile
#if defined CONFIG_RT2860V2_STA || defined CONFIG_RT2860V2_STA_MODULE
	opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	if (!strcmp(opmode, "2") || (!strcmp(opmode, "0") &&
				!strcmp("1", nvram_get(RT2860_NVRAM, "ethConver")))) {
		if (-1 != initStaProfile())
			initStaConnection();
	}
#endif

#ifndef CONFIG_RALINK_RT2880
	if (!strcmp(auth_mode, "Disable") || !strcmp(auth_mode, "OPEN"))
		ledAlways(13, LED_OFF); //turn off security LED (gpio 13)
	else
		ledAlways(13, LED_ON); //turn on security LED (gpio 13)
#endif

#if defined (CONFIG_RT2860V2_AP) || defined (CONFIG_RT2860V2_AP_MODULE)
	restart8021XDaemon(RT2860_NVRAM);	// in wireless.c
#endif
#if defined (CONFIG_INIC_MII) || defined (CONFIG_INIC_PCI) || defined (CONFIG_INIC_USB) 
	restart8021XDaemon(RTINIC_NVRAM);	// in wireless.c
#endif
#if defined (CONFIG_RT2561_AP) || defined (CONFIG_RT2561_AP_MODULE)
	restart8021XDaemon(RT2561_NVRAM);	// in wireless.c
#endif

#ifdef CONFIG_RT2860V2_AP_ANTENNA_DIVERSITY
	AntennaDiversityInit();
#endif

	firewall_init();
	management_init();
	RoutingInit();
#ifdef CONFIG_RALINKAPP_SWQOS
	QoSInit();
#endif

	return 0;
}

static void getMyMAC(webs_t wp, char_t *path, char_t *query)
{
	char myMAC[32];

	arplookup(wp->ipaddr, myMAC);
	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("%s"), myMAC);
	websDone(wp, 200);
}

/* goform/setLan */
static void setLan(webs_t wp, char_t *path, char_t *query)
{
	char_t	*ip, *nm, *dhcp_tp, *stp_en, *lltd_en, *igmp_en, *upnp_en,
			*radvd_en, *pppoer_en, *dnsp_en;
	char_t	*gw = NULL, *pd = NULL, *sd = NULL;
	char_t *lan2enabled, *lan2_ip, *lan2_nm;
#ifdef GA_HOSTNAME_SUPPORT
	char_t	*host;
#endif
	char_t  *dhcp_s, *dhcp_e, *dhcp_m, *dhcp_pd, *dhcp_sd, *dhcp_g, *dhcp_l;
	char_t	*dhcp_sl1, *dhcp_sl2, *dhcp_sl3;
	char	*opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char	*wan_ip = nvram_bufget(RT2860_NVRAM, "wan_ipaddr");
	char	*ctype = nvram_bufget(RT2860_NVRAM, "connectionType");
    char *lanip_raw = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");

	ip = websGetVar(wp, T("lanIp"), T(""));
	nm = websGetVar(wp, T("lanNetmask"), T(""));
	lan2enabled = websGetVar(wp, T("lan2enabled"), T(""));
	lan2_ip = websGetVar(wp, T("lan2Ip"), T(""));
	lan2_nm = websGetVar(wp, T("lan2Netmask"), T(""));
#ifdef GA_HOSTNAME_SUPPORT
	host = websGetVar(wp, T("hostname"), T("0"));
#endif
	dhcp_tp = websGetVar(wp, T("lanDhcpType"), T("DISABLE"));
	stp_en = websGetVar(wp, T("stpEnbl"), T("0"));
	lltd_en = websGetVar(wp, T("lltdEnbl"), T("0"));
	igmp_en = websGetVar(wp, T("igmpEnbl"), T("0"));
	upnp_en = websGetVar(wp, T("upnpEnbl"), T("0"));
	radvd_en = websGetVar(wp, T("radvdEnbl"), T("0"));
	pppoer_en = websGetVar(wp, T("pppoeREnbl"), T("0"));
	dnsp_en = websGetVar(wp, T("dnspEnbl"), T("0"));
	dhcp_s = websGetVar(wp, T("dhcpStart"), T(""));
	dhcp_e = websGetVar(wp, T("dhcpEnd"), T(""));
	dhcp_m = websGetVar(wp, T("dhcpMask"), T(""));
	dhcp_pd = websGetVar(wp, T("dhcpPriDns"), T(""));
	dhcp_sd = websGetVar(wp, T("dhcpSecDns"), T(""));
	dhcp_g = websGetVar(wp, T("dhcpGateway"), T(""));
	dhcp_l = websGetVar(wp, T("dhcpLease"), T("86400"));
	dhcp_sl1 = websGetVar(wp, T("dhcpStatic1"), T(""));
	dhcp_sl2 = websGetVar(wp, T("dhcpStatic2"), T(""));
	dhcp_sl3 = websGetVar(wp, T("dhcpStatic3"), T(""));

	/*
	 * check static ip address:
	 * lan and wan ip should not be the same except in bridge mode
	 */
	if (strncmp(ctype, "STATIC", 7)) {
		if (strcmp(opmode, "0") && !strncmp(ip, wan_ip, 15)) {
			websError(wp, 200, "IP address is identical to WAN");
			return;
		}
		if (!strcmp(lan2enabled, "1"))
		{
			if (strcmp(opmode, "0") && !strncmp(lan2_ip, wan_ip, 15)) {
				websError(wp, 200, "LAN2 IP address is identical to WAN");
				return;
			}
			else if (strcmp(opmode, "0") && !strncmp(lan2_ip, ip, 15)) {
				websError(wp, 200, "LAN2 IP address is identical to LAN1");
				return;
			}
		}
	}
	// configure gateway and dns (WAN) at bridge mode
	if (!strncmp(opmode, "0", 2)) {
		gw = websGetVar(wp, T("lanGateway"), T(""));
		pd = websGetVar(wp, T("lanPriDns"), T(""));
		sd = websGetVar(wp, T("lanSecDns"), T(""));
		nvram_bufset(RT2860_NVRAM, "wan_gateway", gw);
		nvram_bufset(RT2860_NVRAM, "wan_primary_dns", pd);
		nvram_bufset(RT2860_NVRAM, "wan_secondary_dns", sd);
	}
	nvram_bufset(RT2860_NVRAM, "lan_ipaddr", ip);
	nvram_bufset(RT2860_NVRAM, "lan_netmask", nm);
	nvram_bufset(RT2860_NVRAM, "Lan2Enabled", lan2enabled);
	nvram_bufset(RT2860_NVRAM, "lan2_ipaddr", lan2_ip);
	nvram_bufset(RT2860_NVRAM, "lan2_netmask", lan2_nm);
#ifdef GA_HOSTNAME_SUPPORT
	nvram_bufset(RT2860_NVRAM, "HostName", host);
#endif
	if (!strncmp(dhcp_tp, "DISABLE", 8))
		nvram_bufset(RT2860_NVRAM, "dhcpEnabled", "0");
	else if (!strncmp(dhcp_tp, "SERVER", 7)) {
		if (-1 == inet_addr(dhcp_s)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "invalid DHCP Start IP");
			return;
		}
		nvram_bufset(RT2860_NVRAM, "dhcpStart", dhcp_s);
		if (-1 == inet_addr(dhcp_e)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "invalid DHCP End IP");
			return;
		}
		nvram_bufset(RT2860_NVRAM, "dhcpEnd", dhcp_e);
		if (-1 == inet_addr(dhcp_m)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "invalid DHCP Subnet Mask");
			return;
		}
		nvram_bufset(RT2860_NVRAM, "dhcpMask", dhcp_m);
		nvram_bufset(RT2860_NVRAM, "dhcpEnabled", "1");
		nvram_bufset(RT2860_NVRAM, "dhcpPriDns", dhcp_pd);
		nvram_bufset(RT2860_NVRAM, "dhcpSecDns", dhcp_sd);
		nvram_bufset(RT2860_NVRAM, "dhcpGateway", dhcp_g);
		nvram_bufset(RT2860_NVRAM, "dhcpLease", dhcp_l);
		nvram_bufset(RT2860_NVRAM, "dhcpStatic1", dhcp_sl1);
		nvram_bufset(RT2860_NVRAM, "dhcpStatic2", dhcp_sl2);
		nvram_bufset(RT2860_NVRAM, "dhcpStatic3", dhcp_sl3);
	}
	nvram_bufset(RT2860_NVRAM, "stpEnabled", stp_en);
	nvram_bufset(RT2860_NVRAM, "lltdEnabled", lltd_en);
	nvram_bufset(RT2860_NVRAM, "igmpEnabled", igmp_en);
	nvram_bufset(RT2860_NVRAM, "upnpEnabled", upnp_en);
	nvram_bufset(RT2860_NVRAM, "radvdEnabled", radvd_en);
	nvram_bufset(RT2860_NVRAM, "pppoeREnabled", pppoer_en);
	nvram_bufset(RT2860_NVRAM, "dnsPEnabled", dnsp_en);
	nvram_commit(RT2860_NVRAM);
    if(strcmp(lanip_raw, ip) == 0)
	    initInternet();

	//debug print
	websHeader(wp);
	websWrite(wp, T("<h3>LAN Interface Setup</h3><br>\n"));
    
#ifdef GA_HOSTNAME_SUPPORT
	websWrite(wp, T("Hostname: %s<br>\n"), host);
#endif
	websWrite(wp, T("IP: %s<br>\n"), ip);
	websWrite(wp, T("Netmask: %s<br>\n"), nm);
	websWrite(wp, T("LAN2 Enabled: %s<br>\n"), lan2enabled);
	websWrite(wp, T("LAN2 IP: %s<br>\n"), lan2_ip);
	websWrite(wp, T("LAN2 Netmask: %s<br>\n"), lan2_nm);
	if (!strncmp(opmode, "0", 2)) {
		websWrite(wp, T("Gateway: %s<br>\n"), gw);
		websWrite(wp, T("PriDns: %s<br>\n"), pd);

		websWrite(wp, T("SecDns: %s<br>\n"), sd);
	}
	websWrite(wp, T("DHCP type: %s<br>\n"), dhcp_tp);
	if (strncmp(dhcp_tp, "DISABLE", 8)) {
		websWrite(wp, T("--> DHCP start: %s<br>\n"), dhcp_s);
		websWrite(wp, T("--> DHCP end: %s<br>\n"), dhcp_e);
		websWrite(wp, T("--> DHCP mask: %s<br>\n"), dhcp_m);
		websWrite(wp, T("--> DHCP DNS: %s %s<br>\n"), dhcp_pd, dhcp_sd);
		websWrite(wp, T("--> DHCP gateway: %s<br>\n"), dhcp_g);
		websWrite(wp, T("--> DHCP lease: %s<br>\n"), dhcp_l);
		websWrite(wp, T("--> DHCP static 1: %s<br>\n"), dhcp_sl1);
		websWrite(wp, T("--> DHCP static 2: %s<br>\n"), dhcp_sl2);
		websWrite(wp, T("--> DHCP static 3: %s<br>\n"), dhcp_sl3);
	}
	websWrite(wp, T("STP enable: %s<br>\n"), stp_en);
	websWrite(wp, T("LLTD enable: %s<br>\n"), lltd_en);
	websWrite(wp, T("IGMP proxy enable: %s<br>\n"), igmp_en);
	websWrite(wp, T("UPNP enable: %s<br>\n"), upnp_en);
	websWrite(wp, T("RADVD enable: %s<br>\n"), radvd_en);
	websWrite(wp, T("DNS proxy enable: %s<br>\n"), dnsp_en);
    if(strcmp(lanip_raw, ip) != 0)
    {
        websWrite(wp, T("\nLan Ip Address changed, reboot now......<br>\n"));
    }

	websFooter(wp);
	websDone(wp, 200);
    if(strcmp(lanip_raw, ip) != 0)
    {        
        system("sleep 5&&reboot&");        
    }
}

/* goform/setVpnPaThru */
static void setVpnPaThru(webs_t wp, char_t *path, char_t *query)
{
	char_t	*l2tp_pt, *ipsec_pt, *pptp_pt;

	l2tp_pt = websGetVar(wp, T("l2tpPT"), T("0"));
	ipsec_pt = websGetVar(wp, T("ipsecPT"), T("0"));
	pptp_pt = websGetVar(wp, T("pptpPT"), T("0"));
	
	nvram_bufset(RT2860_NVRAM, "l2tpPassThru", l2tp_pt);
	nvram_bufset(RT2860_NVRAM, "ipsecPassThru", ipsec_pt);
	nvram_bufset(RT2860_NVRAM, "pptpPassThru", pptp_pt);
	nvram_commit(RT2860_NVRAM);

	doSystem("vpn-passthru.sh");

	//debug print
	websHeader(wp);
	websWrite(wp, T("<h3>VPN Pass Through</h3><br>\n"));
	websWrite(wp, T("l2tp: %s<br>\n"), l2tp_pt);
	websWrite(wp, T("ipsec: %s<br>\n"), ipsec_pt);
	websWrite(wp, T("pptp: %s<br>\n"), pptp_pt);
	websFooter(wp);
	websDone(wp, 200);
}

int selectOperator(char_t *pName)
{
    int i=0;
    char rec[128];
    char Operator_name[32], nettype[8], dialnum[32], initialcmd[128], user[102], password[102],localip[32],auth_type[5];
    int netype_int;
    char *rules = nvram_bufget(RT2860_NVRAM, "Operators");
    if(!rules)
        return FALSE;
    if(!strlen(rules))
        return FALSE;

    if(!pName)
    {
        return FALSE;
    }
    
    /* format is :
     * [operator name],[user name],[password];
     */
    while(getNthValueSafe(i++, rules, '|', rec, sizeof(rec)) != -1 ){
        // get operator name
        if((getNthValueSafe(0, rec, '!', Operator_name, sizeof(Operator_name)) == -1)){
            continue;
        }

        // get user name
        if((getNthValueSafe(1, rec, '!', nettype, sizeof(nettype)) == -1)){
            continue;
        }

        netype_int = atoi(nettype);

        // get dialing number
        if((getNthValueSafe(2, rec, '!', dialnum, sizeof(dialnum)) == -1)){
            continue;
        }

        // get initial command
        if((getNthValueSafe(3, rec, '!', initialcmd, sizeof(initialcmd)) == -1)){
            continue;
        }
        
        // get user name
        if((getNthValueSafe(4, rec, '!', user, sizeof(user)) == -1)){
            continue;
        }

        // get password
        if((getNthValueSafe(5, rec, '!', password, sizeof(password)) == -1)){
            continue;
        }

        // get local ip
        if((getNthValueSafe(6, rec, '!', localip, sizeof(localip)) == -1)){
            continue;
        }
        // get auth_type //chap or pap
        if((getNthValueSafe(7, rec, '!', auth_type, sizeof(auth_type)) == -1)){
            continue;
        }
        if(strcmp(pName, Operator_name) == 0)
        {
		
            nvram_bufset(RT2860_NVRAM, "g3_operator_name", pName);  
            nvram_bufset(RT2860_NVRAM, "g3_network_type", nettype);
            nvram_bufset(RT2860_NVRAM, "g3_dial_num", dialnum);
            nvram_bufset(RT2860_NVRAM, "g3_initial_cmd", initialcmd);
            nvram_bufset(RT2860_NVRAM, "G3UserName", user);
            nvram_bufset(RT2860_NVRAM, "G3Password", password);
            if(strlen(localip)>6)
				nvram_bufset(RT2860_NVRAM, "G3LocalIp", localip);
			else
				nvram_bufset(RT2860_NVRAM, "G3LocalIp", "");
            nvram_bufset(RT2860_NVRAM, "G3auth_type", auth_type);
            return TRUE;
        }
    }     
    
    return FALSE;   
}


/* goform/setWan */
static void setWan(webs_t wp, char_t *path, char_t *query)
{
	int bConnect = TRUE, bCall = FALSE;
	char_t	*ctype;
	char_t	*ip, *nm, *gw, *pd, *sd;
	char_t	*eth, *user, *pass;
	char_t	*clone_en, *clone_mac;
	char_t  *pptp_srv, *pptp_mode;
	char_t  *l2tp_srv, *l2tp_mode;
#ifdef CONFIG_USER_3G
	char_t	*usb3g_dev;
    char_t  *cur3g_operator;
#endif
	char	*opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char	*lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
	char	*lan2enabled = nvram_bufget(RT2860_NVRAM, "Lan2Enabled");

	ctype = ip = nm = gw = pd = sd = eth = user = pass = 
		clone_en = clone_mac = pptp_srv = pptp_mode = l2tp_srv = l2tp_mode =
		NULL;

	ctype = websGetVar(wp, T("connectionType"), T("0")); 
	if (!strncmp(ctype, "STATIC", 7) || !strcmp(opmode, "0")) {
		//always treat bridge mode having static wan connection
		ip = websGetVar(wp, T("staticIp"), T(""));
		nm = websGetVar(wp, T("staticNetmask"), T("0"));
		gw = websGetVar(wp, T("staticGateway"), T(""));
		pd = websGetVar(wp, T("staticPriDns"), T(""));
		sd = websGetVar(wp, T("staticSecDns"), T(""));

		nvram_bufset(RT2860_NVRAM, "wanConnectionMode", ctype);
		if (-1 == inet_addr(ip)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "invalid IP Address");
			return;
		}
		/*
		 * lan and wan ip should not be the same except in bridge mode
		 */
		if (NULL != opmode && strcmp(opmode, "0") && !strncmp(ip, lan_ip, 15)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "IP address is identical to LAN");
			return;
		}
		if (!strcmp(lan2enabled, "1"))
		{
			char	*lan2_ip = nvram_bufget(RT2860_NVRAM, "lan2_ipaddr");
			if (NULL != opmode && strcmp(opmode, "0") && !strncmp(ip, lan2_ip, 15)) {
				nvram_commit(RT2860_NVRAM);
				websError(wp, 200, "IP address is identical to LAN2");
				return;
			}
		}
		nvram_bufset(RT2860_NVRAM, "wan_ipaddr", ip);
		if (-1 == inet_addr(nm)) {
			nvram_commit(RT2860_NVRAM);
			websError(wp, 200, "invalid Subnet Mask");
			return;
		}
		nvram_bufset(RT2860_NVRAM, "wan_netmask", nm);
		/*
		 * in Bridge Mode, lan and wan are bridged together and associated with
		 * the same ip address
		 */
		if (NULL != opmode && !strcmp(opmode, "0")) {
			nvram_bufset(RT2860_NVRAM, "lan_ipaddr", ip);
			nvram_bufset(RT2860_NVRAM, "lan_netmask", nm);
		}
		nvram_bufset(RT2860_NVRAM, "wan_gateway", gw);
		nvram_bufset(RT2860_NVRAM, "wan_primary_dns", pd);
		nvram_bufset(RT2860_NVRAM, "wan_secondary_dns", sd);
	}
	else if (!strncmp(ctype, "DHCP", 5)) {
		nvram_bufset(RT2860_NVRAM, "wanConnectionMode", ctype);
	}
	else if (!strncmp(ctype, "PPPOE", 6)) {
		char_t *pppoe_opmode, *pppoe_optime,*pppoe_mtu;

		user = websGetVar(wp, T("pppoeUser"), T(""));
		pass = websGetVar(wp, T("pppoePass"), T(""));
		pppoe_opmode = websGetVar(wp, T("pppoeOPMode"), T(""));
		pppoe_mtu = websGetVar(wp, T("pppoe_mtu"), T(""));
		if (0 == strcmp(pppoe_opmode, "OnDemand"))
			pppoe_optime = websGetVar(wp, T("pppoeIdleTime"), T(""));
		else 
			pppoe_optime = websGetVar(wp, T("pppoeRedialPeriod"), T(""));
		nvram_bufset(RT2860_NVRAM, "wan_pppoe_user", user);
		nvram_bufset(RT2860_NVRAM, "wan_pppoe_pass", pass);
		nvram_bufset(RT2860_NVRAM, "wanConnectionMode", ctype);
		nvram_bufset(RT2860_NVRAM, "wan_pppoe_opmode", pppoe_opmode);
		nvram_bufset(RT2860_NVRAM, "wan_pppoe_optime", pppoe_optime);
		nvram_bufset(RT2860_NVRAM, "pppoe_mtu", pppoe_mtu);
	}
	else if (!strncmp(ctype, "L2TP", 5)) {
		char_t *l2tp_opmode, *l2tp_optime;

		l2tp_srv = websGetVar(wp, T("l2tpServer"), T(""));
		user = websGetVar(wp, T("l2tpUser"), T(""));
		pass = websGetVar(wp, T("l2tpPass"), T(""));
		l2tp_mode = websGetVar(wp, T("l2tpMode"), T("0"));
		ip = websGetVar(wp, T("l2tpIp"), T(""));
		nm = websGetVar(wp, T("l2tpNetmask"), T(""));
		gw = websGetVar(wp, T("l2tpGateway"), T(""));
		l2tp_opmode = websGetVar(wp, T("l2tpOPMode"), T(""));
		if (0 == strcmp(l2tp_opmode, "OnDemand"))
			l2tp_optime = websGetVar(wp, T("l2tpIdleTime"), T(""));
		else
			l2tp_optime = websGetVar(wp, T("l2tpRedialPeriod"), T(""));
		nvram_bufset(RT2860_NVRAM, "wanConnectionMode", ctype);
		nvram_bufset(RT2860_NVRAM, "wan_l2tp_server", l2tp_srv);
		nvram_bufset(RT2860_NVRAM, "wan_l2tp_user", user);
		nvram_bufset(RT2860_NVRAM, "wan_l2tp_pass", pass);
		nvram_bufset(RT2860_NVRAM, "wan_l2tp_mode", l2tp_mode);
		nvram_bufset(RT2860_NVRAM, "wan_l2tp_opmode", l2tp_opmode);
		nvram_bufset(RT2860_NVRAM, "wan_l2tp_optime", l2tp_optime);
		if (!strncmp(l2tp_mode, "0", 2)) {
			nvram_bufset(RT2860_NVRAM, "wan_l2tp_ip", ip);
			nvram_bufset(RT2860_NVRAM, "wan_l2tp_netmask", nm);
			nvram_bufset(RT2860_NVRAM, "wan_l2tp_gateway", gw);
		}
	}
	else if (!strncmp(ctype, "PPTP", 5)) {
		char_t *pptp_opmode, *pptp_optime;

		pptp_srv = websGetVar(wp, T("pptpServer"), T(""));
		user = websGetVar(wp, T("pptpUser"), T(""));
		pass = websGetVar(wp, T("pptpPass"), T(""));
		pptp_mode = websGetVar(wp, T("pptpMode"), T("0"));
		ip = websGetVar(wp, T("pptpIp"), T(""));
		nm = websGetVar(wp, T("pptpNetmask"), T(""));
		gw = websGetVar(wp, T("pptpGateway"), T(""));
		pptp_opmode = websGetVar(wp, T("pptpOPMode"), T(""));
		if (0 == strcmp(pptp_opmode, "OnDemand"))
			pptp_optime = websGetVar(wp, T("pptpIdleTime"), T(""));
		else
			pptp_optime = websGetVar(wp, T("pptpRedialPeriod"), T(""));

		nvram_bufset(RT2860_NVRAM, "wanConnectionMode", ctype);
		nvram_bufset(RT2860_NVRAM, "wan_pptp_server", pptp_srv);
		nvram_bufset(RT2860_NVRAM, "wan_pptp_user", user);
		nvram_bufset(RT2860_NVRAM, "wan_pptp_pass", pass);
		nvram_bufset(RT2860_NVRAM, "wan_pptp_mode", pptp_mode);
		nvram_bufset(RT2860_NVRAM, "wan_pptp_opmode", pptp_opmode);
		nvram_bufset(RT2860_NVRAM, "wan_pptp_optime", pptp_optime);
		if (!strncmp(pptp_mode, "0", 2)) {
			nvram_bufset(RT2860_NVRAM, "wan_pptp_ip", ip);
			nvram_bufset(RT2860_NVRAM, "wan_pptp_netmask", nm);
			nvram_bufset(RT2860_NVRAM, "wan_pptp_gateway", gw);
		}
	}
#ifdef CONFIG_USER_3G
	else if (!strncmp(ctype, "3G", 3)) {
        char_t *g3dev, *pin, *g3opmode, *g3idletime, *th, *tm, *tpsel,*cell_mtu;

	g3dev = websGetVar(wp, T("Dev3G"), T("HUAWEI-EM660"));
        pin = websGetVar(wp, T("simPIN"), T(""));
        g3opmode = websGetVar(wp, T("G3Mode"), T("G3ModeAol"));        

            cell_mtu = websGetVar(wp, T("cell_mtu"), T(""));
            nvram_bufset(RT2860_NVRAM, "cell_mtu", cell_mtu);
                
        if(strcmp(g3opmode, T("G3ModeDod")) == 0)
        {
            g3idletime = websGetVar(wp, T("G3IdleTime"), T("0"));
            nvram_bufset(RT2860_NVRAM, "g3idletime", g3idletime);
        }
        else if(strcmp(g3opmode, T("G3ModeDot")) == 0)
        {
		char_t szConn[128], szDisconn[128];
		bCall = FALSE;
            	tpsel = websGetVar(wp, T("timeperiod"), T("100"));
            
			switch(atoi(tpsel))
			{
				case 0:
					th = websGetVar(wp, T("timeperiod0StartHour"), T("0"));
				    tm = websGetVar(wp, T("timeperiod0StartMin"), T("0"));
					sprintf(szConn, "%s %s * * * /sbin/internet.sh", tm, th);
				    nvram_bufset(RT2860_NVRAM, "tp_sh_0", th);
				    nvram_bufset(RT2860_NVRAM, "tp_sm_0", tm);
				    th = websGetVar(wp, T("timeperiod0EndHour"), T("0"));
				    tm = websGetVar(wp, T("timeperiod0EndMin"), T("0"));
					sprintf(szDisconn, "%s %s * * * \"killall -q pppd\"", tm, th);
				    nvram_bufset(RT2860_NVRAM, "tp_eh_0", th);
				    nvram_bufset(RT2860_NVRAM, "tp_em_0", tm);
					break;

				case 1:
					th = websGetVar(wp, T("timeperiod1StartHour"), T("0"));
				    tm = websGetVar(wp, T("timeperiod1StartMin"), T("0"));
					sprintf(szConn, "%s %s * * * /sbin/internet.sh", tm, th);
				    nvram_bufset(RT2860_NVRAM, "tp_sh_1", th);
				    nvram_bufset(RT2860_NVRAM, "tp_sm_1", tm);
				    th = websGetVar(wp, T("timeperiod1EndHour"), T("0"));
				    tm = websGetVar(wp, T("timeperiod1EndMin"), T("0"));
					sprintf(szDisconn, "%s %s * * * \"killall -q pppd\"", tm, th);
				    nvram_bufset(RT2860_NVRAM, "tp_eh_1", th);
				    nvram_bufset(RT2860_NVRAM, "tp_em_1", tm);
					break;

				case 2:
					th = websGetVar(wp, T("timeperiod2StartHour"), T("0"));
				    tm = websGetVar(wp, T("timeperiod2StartMin"), T("0"));
					sprintf(szConn, "%s %s * * * /sbin/internet.sh", tm, th);
				    nvram_bufset(RT2860_NVRAM, "tp_sh_2", th);
				    nvram_bufset(RT2860_NVRAM, "tp_sm_2", tm);
				    th = websGetVar(wp, T("timeperiod2EndHour"), T("0"));
				    tm = websGetVar(wp, T("timeperiod2EndMin"), T("0"));
					sprintf(szDisconn, "%s %s * * * \"killall -q pppd\"", tm, th);
				    nvram_bufset(RT2860_NVRAM, "tp_eh_2", th);
				    nvram_bufset(RT2860_NVRAM, "tp_em_2", tm);
					break;
		
				case 3:
					th = websGetVar(wp, T("timeperiod3StartHour"), T("0"));
				    tm = websGetVar(wp, T("timeperiod3StartMin"), T("0"));
					sprintf(szConn, "%s %s * * * /sbin/internet.sh", tm, th);
				    nvram_bufset(RT2860_NVRAM, "tp_sh_3", th);
				    nvram_bufset(RT2860_NVRAM, "tp_sm_3", tm);
				    th = websGetVar(wp, T("timeperiod3EndHour"), T("0"));
				    tm = websGetVar(wp, T("timeperiod3EndMin"), T("0"));
					sprintf(szDisconn, "%s %s * * * \"killall -q pppd\"", tm, th);
				    nvram_bufset(RT2860_NVRAM, "tp_eh_3", th);
				    nvram_bufset(RT2860_NVRAM, "tp_em_3", tm);
					break;

				default:
					websHeader(wp);
					websWrite(wp, T("<h2>Unknown time period selected: %s</h2><br>\n"), tpsel);
					websFooter(wp);
					websDone(wp, 200);
					return;
			}

			nvram_bufset(RT2860_NVRAM, "DOT_select_index", tpsel);
        	}
		else if(strcmp(g3opmode, T("G3ModeManual")) == 0)
		{
			char_t *pConn = websGetVar(wp, T("g3btnconnect"), NULL);
			if(!pConn)
			{
				// just set as muanual mode, not to connect or disconnect	
				bConnect = TRUE;
				bCall = FALSE;
			}
			else if(strcmp(pConn, "connect") == 0)
			{
				bCall = TRUE;
			}
			else if(strcmp(pConn, "disconnect") == 0)
			{
				doSystem("killall -q pppd");
				bConnect = FALSE;
			}
			else
			{
				websRedirect(wp, "internet/wan.asp");
				return;
			}
		}
		else if(strcmp(g3opmode, T("G3ModeAol")) == 0)
		{
			
		}
		else
		{
			websHeader(wp);
			websWrite(wp, T("<h2>Unknown 3g Connection Mode: %s</h2><br>\n"), g3opmode);
			websFooter(wp);
			websDone(wp, 200);
			return;
		}
        
		nvram_bufset(RT2860_NVRAM, "wan_3g_dev", g3dev);
		nvram_bufset(RT2860_NVRAM, "g3_sim_pin", pin);
        	nvram_bufset(RT2860_NVRAM, "wan_3g_opmode", g3opmode);
		nvram_bufset(RT2860_NVRAM, "wanConnectionMode", ctype);   
		initDeviceName();   
	}
#endif
	else {
		websHeader(wp);
		websWrite(wp, T("<h2>Unknown Connection Type: %s</h2><br>\n"), ctype);
		websFooter(wp);
		websDone(wp, 200);
		return;
	}

	// mac clone
	clone_en = websGetVar(wp, T("macCloneEnbl"), T("0"));
	clone_mac = websGetVar(wp, T("macCloneMac"), T(""));
	nvram_bufset(RT2860_NVRAM, "macCloneEnabled", clone_en);
	if (!strncmp(clone_en, "1", 2))
		nvram_bufset(RT2860_NVRAM, "macCloneMac", clone_mac);
	nvram_commit(RT2860_NVRAM);

	if(bConnect){
		initInternet();
		sleep(3);
		doSystem("start_linkbackup >/dev/console &");
	}

	if(bCall)
	{
		doSystem("pppd call 3g");
	}

	// debug print
	websRedirect(wp, "internet/wan.asp");
}

int checkOperator(char_t *pName)
{
    int i;
	char_t rec[128], Operator_name[32];
	char_t *value, *operatorSel;

    char *rules = nvram_bufget(RT2860_NVRAM, "Operators");
    if(!rules || !strlen(rules) )
        return FALSE;

    // delete from list
    i = 0;
    while(getNthValueSafe(i++, rules, '|', rec, sizeof(rec)) != -1 )
    {
		if((getNthValueSafe(0, rec, '!', Operator_name, sizeof(Operator_name)) == -1))
        {
			continue;
		}

        if(strcmp(Operator_name, pName) == 0)
        {
            return TRUE;
        }
	}    

    return FALSE;
}

static void Operators(webs_t wp, char_t *path, char_t *query)
{
	char rule[8192];
    char *Operator_name, *nettype, *dialnum, *initialcmd, *user, *password, *authtype, *compress,*localip;
    char *operators;
    int nettype_int, authtype_int, bcompress;

    Operator_name = websGetVar(wp, T("G3Comment"), T(""));    
    nettype = websGetVar(wp, T("g3NetworkType"), T(""));
    dialnum = websGetVar(wp, T("DialingStr"), T(""));
    initialcmd = websGetVar(wp, T("InitCmdStr"), T(""));
    user = websGetVar(wp, T("G3UserName"), T(""));
    password = websGetVar(wp, T("G3Password"), T(""));
    authtype = websGetVar(wp, T("G3AuthProtocol"), T(""));
    compress = websGetVar(wp, T("G3UseCompress"), NULL);
    localip = websGetVar(wp, T("G3LocalIp"), T(""));

    if(!Operator_name && !strlen(Operator_name))
		return;

    if(checkOperator(Operator_name))
    {
        websHeader(wp);
		websWrite(wp, T("<h2>Error: a MSP with this name %s exist</h2><br>\n"), Operator_name);
		websFooter(wp);
		websDone(wp, 200);
        return;
    }

    if(!nettype && !strlen(nettype))
		return;

    nettype_int = atoi(nettype);
    if(nettype_int > 3)
        return;

    authtype_int = atoi(authtype);
    if(authtype_int >= 3)
        return;

    if(compress)
    {
        bcompress = 1;
    }
    else
    {
        bcompress = 0;
    }    
    
    if(!user)
		return;

    if(!dialnum)
        return;

    if(!initialcmd)
        return;

    if(!password)
		return;


    if(!localip)
		return;

	if(strlen(Operator_name) > 32)
		return;

    if(strlen(dialnum) > 32)
		return;

    if(strlen(initialcmd) > 128)
		return;

    if(strlen(user) > 32)
		return;

    if(strlen(password) > 32)
		return;
    if(strlen(localip) > 32)
		return;
    
	/* i know you will try to break our box... ;) */
	if(strchr(Operator_name, '!') || strchr(Operator_name, '|'))
		return;

    if(strchr(dialnum, '!') || strchr(dialnum, '|'))
		return;

    if(strchr(initialcmd, '!') || strchr(initialcmd, '|'))
		return;
    
    if(strchr(user, '!') || strchr(user, '|'))
		return;

    if(strchr(password, '!') || strchr(password, '|'))
		return;
    if(strchr(localip, '!') || strchr(localip, '|'))
		return;

	if(( operators = nvram_bufget(RT2860_NVRAM, "Operators")) && strlen( operators) )
		snprintf(rule, sizeof(rule), "%s|%s!%d!%s!%s!%s!%s!%s!%d!%d",  operators, 
		    Operator_name, nettype_int, dialnum, initialcmd, user, password,localip, authtype_int, bcompress);
	else
		snprintf(rule, sizeof(rule), "%s!%d!%s!%s!%s!%s!%s!%d!%d",
		    Operator_name, nettype_int, dialnum, initialcmd, user, password,localip, authtype_int, bcompress);

	nvram_set(RT2860_NVRAM, "Operators", rule);
	selectOperator(Operator_name);
/*
	nvram_bufset(RT2860_NVRAM, "g3_operator_name", Operator_name);
    nvram_bufset(RT2860_NVRAM, "g3_network_type", nettype);
    nvram_bufset(RT2860_NVRAM, "g3_dial_num", dialnum);
    nvram_bufset(RT2860_NVRAM, "g3_initial_cmd", initialcmd);
    nvram_bufset(RT2860_NVRAM, "G3UserName", user);
    nvram_bufset(RT2860_NVRAM, "G3Password", password);
    nvram_bufset(RT2860_NVRAM, "G3LocalIp", localip);
	nvram_bufset(RT2860_NVRAM, "G3AuthType", authtype);
    nvram_bufset(RT2860_NVRAM, "G3bCompressed", compress);
*/
	nvram_commit(RT2860_NVRAM);

end:
	websRedirect(wp, "internet/wan.asp");        
}

/*
 * ASP function
 */
static int showOperatorsASP(int eid, webs_t wp, int argc, char_t **argv)
{
	int i=0;
	char rec[128];
    char Operator_name[32], nettype[8], dialnum[32], initialcmd[128], user[100], password[100], authtype[8], compress[8],localip[32];
    int netype_int, authtype_int, bcompress;
	char *rules = nvram_bufget(RT2860_NVRAM, "Operators");
	char*  p;
	char cur_operator_name[32];
	p=nvram_bufget(RT2860_NVRAM,"g3_operator_name");
	memset(cur_operator_name,0,sizeof(cur_operator_name));	
	strcpy(cur_operator_name,p);

	if(!rules)
		return 0;
	if(!strlen(rules))
		return 0;

	/* format is :
	 * [operator name],[user name],[password];
	 */
	while(getNthValueSafe(i++, rules, '|', rec, sizeof(rec)) != -1 ){
		// get operator name
		if((getNthValueSafe(0, rec, '!', Operator_name, sizeof(Operator_name)) == -1)){
			continue;
		}

        // get net type
		if((getNthValueSafe(1, rec, '!', nettype, sizeof(nettype)) == -1)){
			continue;
		}

        netype_int = atoi(nettype);

		// get dialing number
		if((getNthValueSafe(2, rec, '!', dialnum, sizeof(dialnum)) == -1)){
			continue;
		}

        // get initial command
		if((getNthValueSafe(3, rec, '!', initialcmd, sizeof(initialcmd)) == -1)){
			continue;
		}
        
		// get user name
		if((getNthValueSafe(4, rec, '!', user, sizeof(user)) == -1)){
			continue;
		}

		// get password
		if((getNthValueSafe(5, rec, '!', password, sizeof(password)) == -1)){
			continue;
		}
		// get local ip
		if((getNthValueSafe(6, rec, '!', localip, sizeof(localip)) == -1)){
			continue;
		}

        if((getNthValueSafe(7, rec, '!', authtype, sizeof(authtype)) == -1)){
			continue;
		}

        authtype_int = atoi(authtype);

        if((getNthValueSafe(8, rec, '!', compress, sizeof(compress)) == -1)){
			continue;
		}
/*
		p = nvram_bufget(RT2860_NVRAM, "g3_network_type");
		strncpy(nettype, p, sizeof(nettype));
		p = nvram_bufget(RT2860_NVRAM, "g3_dial_num");
		strncpy(dialnum, p, sizeof(dialnum));
		p = nvram_bufget(RT2860_NVRAM, "g3_initial_cmd");
		strncpy(initialcmd, p, sizeof(initialcmd));
		p = nvram_bufget(RT2860_NVRAM, "G3UserName");
		strncpy(user, p, sizeof(user));
		p = nvram_bufget(RT2860_NVRAM, "G3Password");
		strncpy(password, p, sizeof(password));
		p = nvram_bufget(RT2860_NVRAM, "G3LocalIp");
		strncpy(localip, p, sizeof(localip));
		p = nvram_bufget(RT2860_NVRAM, "G3AuthType");
		strncpy(authtype, p, sizeof(authtype));
		p = nvram_bufget(RT2860_NVRAM, "G3bCompressed");
		strncpy(compress, p, sizeof(compress));
*/
        bcompress = atoi(compress);

		websWrite(wp, T("<tr>\n"));
	// output No.
	if(!strcmp(cur_operator_name,Operator_name))
		websWrite(wp, T("<td> <input type=\"radio\" checked  name=\"RuleNo\" value=\"%s\" onclick=\"MSPListOnSelect(%d)\"> </td>"),Operator_name, i-1); 
	else
		websWrite(wp, T("<td> <input type=\"radio\" name=\"RuleNo\" value=\"%s\" onclick=\"MSPListOnSelect(%d)\"> </td>"),Operator_name, i-1);

        // output operator name
		websWrite(wp, T("<td align=center id=\"Name%d\" value=\"%s\">%s</td>"), i-1, Operator_name, Operator_name);
        /*
        switch(netype_int)
        {
            case 0:
                websWrite(wp, T("<td align=center id=\"NetType%d\">EVDORevApreferred</td>"), i-1);
                break;
                
            case 1:
                websWrite(wp, T("<td align=center id=\"NetType%d\">EVDORevAonly</td>"), i-1);
                break;
                
            case 2:
                websWrite(wp, T("<td align=center id=\"NetType%d\">CDMA1Xonly</td>"), i-1);
                break;             
        }
        */

        if(strlen(dialnum))
            websWrite(wp, T("<td align=center id=\"DialNum%d\">%s</td>"), i-1, dialnum);
        else
			websWrite(wp, T("<td align=center id=\"DialNum%d\">&nbsp;</td>"), i-1);

        if(strlen(initialcmd))
            websWrite(wp, T("<td align=center id=\"InitCmd%d\">%s</td>"), i-1, initialcmd);
        else
			websWrite(wp, T("<td align=center id=\"InitCmd%d\">&nbsp;</td>"), i-1);
        
		// output user name
		if(strlen(user))
			websWrite(wp, T("<td align=center id=\"User%d\">%s</td>"), i-1, user);
        else
			websWrite(wp, T("<td align=center id=\"User%d\">&nbsp;</td>"), i-1);
        
        // output user name
		if(strlen(password))
			websWrite(wp, T("<td align=center id=\"Password%d\">%s</td>"), i-1, password);        
		else
			websWrite(wp, T("<td align=center id=\"Password%d\">&nbsp;</td>"), i-1);
		// output local ip        
		if(strlen(localip)>6)
			websWrite(wp, T("<td align=center id=\"LocalIp%d\">%s</td>"), i-1, localip);        
		else
			websWrite(wp, T("<td align=center id=\"LocalIp%d\">&nbsp;</td>"), i-1);

        websWrite(wp, T("<td align=center> <input type=\"submit\" name=\"DelRule\" value=\"Delete\"> <br />"));
        websWrite(wp, T("<input type=\"hidden\" id=\"NetType%d\" value=\"%d\">"), i-1, netype_int);
        websWrite(wp, T("<input type=\"hidden\" id=\"AuthType%d\" value=\"%d\">"), i-1, authtype_int);
        websWrite(wp, T("<input type=\"hidden\" id=\"Bcomp%d\" value=\"%d\"> </td>"), i-1, bcompress);
        
		websWrite(wp, T("</tr>\n"));
	}

    websWrite(wp, T("<tr>\n"));
    websWrite(wp, T("<td> <input type=\"hidden\" name=\"totalMSP\" value=\"%d\"> </td>"), i);
    websWrite(wp, T("</tr>\n"));
    
	return 0;	
}

static int showNetworkTypeASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char *pSubDev;

	pSubDev = nvram_bufget(RT2860_NVRAM, "wan_3g_dev");
	if((strcmp(pSubDev, "HUAWEI-EM660") == 0)||(strcmp(pSubDev,"KSE-360")==0))
	{
		websWrite(wp, T("<option value=0>CDMA/EVDO HYBRID</option>\n"));
		websWrite(wp, T("<option value=1>EVDO Only</option>\n"));
		websWrite(wp, T("<option value=2>CDMA Only</option>\n"));
	}
	else if(strcmp(pSubDev, "HUAWEI-EM770") == 0)
	{
		websWrite(wp, T("<option value=0>Automatic search</option>\n"));
		websWrite(wp, T("<option value=1>WCDMA ONLY</option>\n"));
		websWrite(wp, T("<option value=2>GSM ONLY</option>\n"));
		websWrite(wp, T("<option value=3>No change</option>\n"));
	}
	else if(strcmp(pSubDev, "HUAWEI-EM560") == 0)
	{
		websWrite(wp, T("<option value=0>Automatic search</option>\n"));
		websWrite(wp, T("<option value=1>TDSCDMA  ONLY</option>\n"));
		websWrite(wp, T("<option value=2>WCDMA  ONLY</option>\n"));
		websWrite(wp, T("<option value=3>GSM ONLY</option>\n"));
	}
	else if(strcmp(pSubDev, "THINKWILL-MI600") == 0)
	{
		websWrite(wp, T("<option value=0>CDMA/EVDO HYBRID</option>\n"));
		websWrite(wp, T("<option value=1>EVDO Only</option>\n"));
		websWrite(wp, T("<option value=2>CDMA Only</option>\n"));
	}
	else if(strcmp(pSubDev, "IE901D") == 0)
        {
                websWrite(wp, T("<option value=0>CDMA/EVDO HYBRID</option>\n"));
                websWrite(wp, T("<option value=1>EVDO Only</option>\n"));
                websWrite(wp, T("<option value=2>CDMA Only</option>\n"));
        }
	else if(strcmp(pSubDev, "SYNCWISER-801/401") == 0)
	{
		websWrite(wp, T("<option value=0>CDMA/EVDO HYBRID</option>\n"));
		websWrite(wp, T("<option value=1>EVDO Only</option>\n"));
		websWrite(wp, T("<option value=2>CDMA Only</option>\n"));
	}
	else if(strcmp(pSubDev, "LONGSUNG-C5300") == 0)
	{
		websWrite(wp, T("<option value=0>CDMA/EVDO HYBRID</option>\n"));
		websWrite(wp, T("<option value=1>EVDO Only</option>\n"));
		websWrite(wp, T("<option value=2>CDMA Only</option>\n"));
	}
	else if(strcmp(pSubDev, "LONGSUNG-U6300/U5300") == 0)
	{
		websWrite(wp, T("<option value=0>WCDMA PREFERRED</option>\n"));
		websWrite(wp, T("<option value=1>WCDMA Only</option>\n"));
		websWrite(wp, T("<option value=2>GSM PREFERRED</option>\n"));
		websWrite(wp, T("<option value=3>GSM Only</option>\n"));
	}
	else if(strcmp(pSubDev, "GAORAN-280") == 0)
	{
		websWrite(wp, T("<option value=0>Automatic search</option>\n"));
		websWrite(wp, T("<option value=1>WCDMA ONLY</option>\n"));
		websWrite(wp, T("<option value=2>GSM ONLY</option>\n"));
	}
	else if(strcmp(pSubDev, "TW-W1M100") == 0)
	{
		websWrite(wp, T("<option value=0>WCDMA PREFERRED</option>\n"));
		websWrite(wp, T("<option value=1>WCDMA Only</option>\n"));
		websWrite(wp, T("<option value=2>GSM PREFERRED</option>\n"));
		websWrite(wp, T("<option value=3>GSM Only</option>\n"));
	}
	else if(strcmp(pSubDev, "MC5728") == 0)
	{
		websWrite(wp, T("<option value=0>CDMA/EVDO HYBRID</option>\n"));
	}
	else if(strcmp(pSubDev, "SIMCOM-SIM700") == 0)
	{
		websWrite(wp, T("<option value=0>GSM/EDGE</option>\n"));
	}
	else if(strcmp(pSubDev, "ZTE-MU301") == 0)
	{
		websWrite(wp, T("<option value=0>Automatic search</option>\n"));
		websWrite(wp, T("<option value=1>TDSCDMA  ONLY</option>\n"));
		websWrite(wp, T("<option value=2>WCDMA  ONLY</option>\n"));
		websWrite(wp, T("<option value=3>GSM ONLY</option>\n"));
	}
	else if(strcmp(pSubDev, "ZTE-MF210V") == 0)
	{
		websWrite(wp, T("<option value=0>Automatic search</option>\n"));
		websWrite(wp, T("<option value=1>WCDMA ONLY</option>\n"));
		websWrite(wp, T("<option value=2>GSM ONLY</option>\n"));
	}
	else if(strcmp(pSubDev, "ZX-600") == 0)
	{
		websWrite(wp, T("<option value=0>AUTO MODE</option>\n"));
		websWrite(wp, T("<option value=1>WCDMA PREFERRED</option>\n"));
		websWrite(wp, T("<option value=2>WCDMA Only</option>\n"));
		websWrite(wp, T("<option value=3>GSM PREFERRED</option>\n"));
		websWrite(wp, T("<option value=4>GSM Only</option>\n"));
	}
	  else if((strcmp(pSubDev,"SIERRA-MC8785")==0) || (strcmp(pSubDev,"GTM681W")==0)){
                websWrite(wp, T("<option value=0>AUTO</option>\n"));
        }
    else
    {
        websWrite(wp, T("<option value=0>AUTO</option>\n"));
    }

}

static int showMuanalBtnASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char if_addr[16];

	if (-1 == getIfIp(getWanIfNamePPP(), if_addr)) {
		//websError(wp, 500, T("getWanIp: calling getIfIp error\n"));
		return websWrite(wp, T("<input type=\"submit\" name=\"g3btnconnect\" id = \"g3connectbtn\" value=\"%s\">"), "connect");
	}
	
	if(if_addr[0])
	{
		return websWrite(wp, T("<input type=\"submit\" name=\"g3btnconnect\" id = \"g3connectbtn\" value=\"%s\">"), "disconnect");
	}
	else
	{
		return websWrite(wp, T("<input type=\"submit\" name=\"g3btnconnect\" id = \"g3connectbtn\" value=\"%s\">"), "connect");
	}
}

static void OperatorDelete(webs_t wp, char_t *path, char_t *query)
{
	int i, j, rule_count;
	char_t name_buf[16], rec[128], Operator_name[32];
	char_t *value, *operatorSel, *curOperator;
	int *deleArray;
	char *firewall_enable;

    char *rules = nvram_bufget(RT2860_NVRAM, "Operators");
    if(!rules || !strlen(rules) )
        return;

    curOperator = nvram_bufget(RT2860_NVRAM, "g3_operator_name");
    // get current selected operator name
    operatorSel = websGetVar(wp, T("RuleNo"), T(""));
    if(operatorSel == T(""))
    {
        websHeader(wp);
		websWrite(wp, T("<h2>MSP Error: %s</h2><br>\n"), operatorSel);
		websFooter(wp);
		websDone(wp, 200);
        return;
    }

    value = websGetVar(wp, T("setDefaultMSP"), NULL);
    if(value)
    {
        // set default msp        
        if((!selectOperator(operatorSel)))
        {
            websHeader(wp);
    		websWrite(wp, T("<h2>MSP Error: %s</h2><br>\n"), operatorSel);
    		websFooter(wp);
    		websDone(wp, 200);
            return;
        } 
    }

    rule_count = 1;
    deleArray = (int *)malloc(rule_count * sizeof(int));
    if(!deleArray)
    {
        return;
    }

    value = websGetVar(wp, T("DelRule"), NULL);
    if(value)
    {
        // delete from list
        i = 0;
        j = 0;
        while(getNthValueSafe(i++, rules, '|', rec, sizeof(rec)) != -1 )
        {
    		if((getNthValueSafe(0, rec, '!', Operator_name, sizeof(Operator_name)) == -1))
            {
    			continue;
    		}
            else if(strcmp(Operator_name, operatorSel) == 0)
            {
                deleArray[j++] = i - 1;
                if(strcmp(curOperator, operatorSel) == 0)
                {
                    nvram_bufset(RT2860_NVRAM, "g3_operator_name", T(""));
                    nvram_bufset(RT2860_NVRAM, "g3_network_type", T(""));
                    nvram_bufset(RT2860_NVRAM, "g3_dial_num", T(""));
                    nvram_bufset(RT2860_NVRAM, "g3_initial_cmd", T(""));
                    nvram_bufset(RT2860_NVRAM, "G3UserName", T(""));
                    nvram_bufset(RT2860_NVRAM, "G3Password", T(""));
                    nvram_bufset(RT2860_NVRAM, "G3LocalIp", T(""));
                }
                
                break;
            }
    	}
    }
    
    if(!j){
        free(deleArray);
        websHeader(wp);
        websWrite(wp, T("You didn't select any rules to delete.<br>\n"));
        websFooter(wp);
        websDone(wp, 200);
        return;
    }

	deleteNthValueMulti(deleArray, rule_count, rules, '|');
	free(deleArray);
	nvram_set(RT2860_NVRAM, "Operators", rules);
	nvram_commit(RT2860_NVRAM);

    websRedirect(wp, "internet/wan.asp");

	return;
}

