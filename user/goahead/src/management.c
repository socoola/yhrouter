#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <linux/if.h>
#include <linux/wireless.h>
#include <ctype.h>
#include <errno.h>
#include "linux/autoconf.h"
#include "config/autoconf.h" //user config
#include "user/busybox/include/autoconf.h" //busybox config

#ifdef USER_MANAGEMENT_SUPPORT
#include "um.h"
#endif
#include "nvram.h"
#include "utils.h"
#include "webs.h"
#include "internet.h"
#include "wireless.h"

#include "management.h"
#include "wps.h"

extern void WPSRestart(void);
extern void formDefineWPS(void);


#define COMMAND_MAX	1024
static char system_command[COMMAND_MAX];


/*
 * goform/setSysAdm
 */
static void setSysAdm(webs_t wp, char_t *path, char_t *query)
{
	char_t *admuser, *admpass;
	char *old_user;

	old_user = nvram_bufget(RT2860_NVRAM, "Login");
	admuser = websGetVar(wp, T("admuser"), T(""));
	admpass = websGetVar(wp, T("admpass"), T(""));

	if (!strlen(admuser)) {
		error(E_L, E_LOG, T("setSysAdm: account empty, leave it unchanged"));
		return;
	}
	if (!strlen(admpass)) {
		error(E_L, E_LOG, T("setSysAdm: password empty, leave it unchanged"));
		return;
	}
	nvram_bufset(RT2860_NVRAM, "Login", admuser);
	nvram_bufset(RT2860_NVRAM, "Password", admpass);
	nvram_commit(RT2860_NVRAM);

	/* modify /etc/passwd to new user name and passwd */
/*
	doSystem("sed -e 's/^%s:/%s:/' /etc/passwd > /etc/newpw", old_user, admuser);
	doSystem("cp /etc/newpw /etc/passwd");
	doSystem("rm -f /etc/newpw");
	doSystem("chpasswd.sh %s %s", admuser, admpass);
*/

#ifdef USER_MANAGEMENT_SUPPORT
	if (umGroupExists(T("adm")) == FALSE)
		umAddGroup(T("adm"), 0x07, AM_DIGEST, FALSE, FALSE);
	if (old_user != NULL && umUserExists(old_user))
		umDeleteUser(old_user);
	if (umUserExists(admuser))
		umDeleteUser(admuser);
	umAddUser(admuser, admpass, T("adm"), FALSE, FALSE);
#endif

	websHeader(wp);
	websWrite(wp, T("<h2>Adminstrator Settings</h2><br>\n"));
	websWrite(wp, T("adm user: %s<br>\n"), admuser);
	websWrite(wp, T("adm pass: %s<br>\n"), admpass);
	websFooter(wp);
	websDone(wp, 200);        
}

/*
 * goform/setSysLang
 */
static void setSysLang(webs_t wp, char_t *path, char_t *query)
{
	char_t *lang;

	lang = websGetVar(wp, T("langSelection"), T(""));
	nvram_bufset(RT2860_NVRAM, "Language", lang);
	nvram_commit(RT2860_NVRAM);

	websHeader(wp);
	websWrite(wp, T("<h2>Language Selection</h2><br>\n"));
	websWrite(wp, T("language: %s<br>\n"), lang);
	websFooter(wp);
	websDone(wp, 200);        
}

/*
 * goform/NTP
 */
static void NTP(webs_t wp, char_t *path, char_t *query)
{
	char *tz, *ntpServer, *ntpSync;

	tz = websGetVar(wp, T("time_zone"), T(""));
	ntpServer = websGetVar(wp, T("NTPServerIP"), T(""));
	ntpSync = websGetVar(wp, T("NTPSync"), T(""));

	if(!tz || !ntpServer || !ntpSync)
		return;

	if(!strlen(tz))
		return;

	if(checkSemicolon(tz))
		return;

	if(!strlen(ntpServer)){
		// user choose to make  NTP server disable
		nvram_bufset(RT2860_NVRAM, "NTPServerIP", "");
		nvram_bufset(RT2860_NVRAM, "NTPSync", "");
	}else{
		if(checkSemicolon(ntpServer))
			return;
		if(!strlen(ntpSync))
			return;
		if(atoi(ntpSync) > 300)
			return;
		nvram_bufset(RT2860_NVRAM, "NTPServerIP", ntpServer);
		nvram_bufset(RT2860_NVRAM, "NTPSync", ntpSync);
	}
	nvram_bufset(RT2860_NVRAM, "TZ", tz);
	nvram_commit(RT2860_NVRAM);

	doSystem("ntp.sh");

	websHeader(wp);
	websWrite(wp, T("<h2>NTP Settings</h2><br>\n"));
	websWrite(wp, T("NTPserver: %s<br>\n"), ntpServer);
	websWrite(wp, T("TZ: %s<br>\n"), tz);
	websWrite(wp, T("NTPSync: %s<br>\n"), ntpSync);
	websFooter(wp);
	websDone(wp, 200);        
}

#ifdef CONFIG_DATE
/*
 * goform/NTPSyncWithHost
 */
static void NTPSyncWithHost(webs_t wp, char_t *path, char_t *query)
{
	if(!query || (!strlen(query)))
		return;
	if(strchr(query, ';'))
		return;

	doSystem("date -s %s", query);


	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));
	websWrite(wp, T("n/a"));
	websDone(wp, 200);
}
#endif

#ifdef CONFIG_USER_GOAHEAD_GreenAP
/*
 * goform/GreenAP
 */
static void GreenAP(webs_t wp, char_t *path, char_t *query)
{
	char_t *shour1, *sminute1, *ehour1, *eminute1, *action1;
	char_t *shour2, *sminute2, *ehour2, *eminute2, *action2;
	char_t *shour3, *sminute3, *ehour3, *eminute3, *action3;
	char_t *shour4, *sminute4, *ehour4, *eminute4, *action4;
	char start[6], end[6];
	shour1 = websGetVar(wp, T("GAPSHour1"), T(""));
	sminute1 = websGetVar(wp, T("GAPSMinute1"), T(""));
	ehour1 = websGetVar(wp, T("GAPEHour1"), T(""));
	eminute1 = websGetVar(wp, T("GAPEMinute1"), T(""));
	action1 = websGetVar(wp, T("GAPAction1"), T(""));
	sprintf(start, "%s %s", sminute1, shour1);
	sprintf(end, "%s %s", eminute1, ehour1);
	nvram_bufset(RT2860_NVRAM, "GreenAPStart1", start);
	nvram_bufset(RT2860_NVRAM, "GreenAPEnd1", end);
	nvram_bufset(RT2860_NVRAM, "GreenAPAction1", action1);
	shour2 = websGetVar(wp, T("GAPSHour2"), T(""));
	sminute2 = websGetVar(wp, T("GAPSMinute2"), T(""));
	ehour2 = websGetVar(wp, T("GAPEHour2"), T(""));
	eminute2 = websGetVar(wp, T("GAPEMinute2"), T(""));
	action2 = websGetVar(wp, T("GAPAction2"), T(""));
	sprintf(start, "%s %s", sminute2, shour2);
	sprintf(end, "%s %s", eminute2, ehour2);
	nvram_bufset(RT2860_NVRAM, "GreenAPStart2", start);
	nvram_bufset(RT2860_NVRAM, "GreenAPEnd2", end);
	nvram_bufset(RT2860_NVRAM, "GreenAPAction2", action2);
	shour3 = websGetVar(wp, T("GAPSHour3"), T(""));
	sminute3 = websGetVar(wp, T("GAPSMinute3"), T(""));
	ehour3 = websGetVar(wp, T("GAPEHour3"), T(""));
	eminute3 = websGetVar(wp, T("GAPEMinute3"), T(""));
	action3 = websGetVar(wp, T("GAPAction3"), T(""));
	sprintf(start, "%s %s", sminute3, shour3);
	sprintf(end, "%s %s", eminute3, ehour3);
	nvram_bufset(RT2860_NVRAM, "GreenAPStart3", start);
	nvram_bufset(RT2860_NVRAM, "GreenAPEnd3", end);
	nvram_bufset(RT2860_NVRAM, "GreenAPAction3", action3);
	shour4 = websGetVar(wp, T("GAPSHour4"), T(""));
	sminute4 = websGetVar(wp, T("GAPSMinute4"), T(""));
	ehour4 = websGetVar(wp, T("GAPEHour4"), T(""));
	eminute4 = websGetVar(wp, T("GAPEMinute4"), T(""));
	action4 = websGetVar(wp, T("GAPAction4"), T(""));
	sprintf(start, "%s %s", sminute4, shour4);
	sprintf(end, "%s %s", eminute4, ehour4);
	nvram_bufset(RT2860_NVRAM, "GreenAPStart4", start);
	nvram_bufset(RT2860_NVRAM, "GreenAPEnd4", end);
	nvram_bufset(RT2860_NVRAM, "GreenAPAction4", action4);
	nvram_commit(RT2860_NVRAM);

	doSystem("greenap.sh init");

	websHeader(wp);
	websWrite(wp, T("GreenAPStart1: %s %s<br>\n"), sminute1, shour1);
	websWrite(wp, T("GreenAPEnd1: %s %s<br>\n"), eminute1, ehour1);
	websWrite(wp, T("GreenAPAction1: %s<br>\n"), action1);
	websWrite(wp, T("GreenAPStart2: %s %s<br>\n"), sminute2, shour2);
	websWrite(wp, T("GreenAPEnd2: %s %s<br>\n"), eminute2, ehour2);
	websWrite(wp, T("GreenAPAction2: %s<br>\n"), action2);
	websWrite(wp, T("GreenAPStart3: %s %s<br>\n"), sminute3, shour3);
	websWrite(wp, T("GreenAPEnd3: %s %s<br>\n"), eminute3, ehour3);
	websWrite(wp, T("GreenAPAction3: %s<br>\n"), action3);
	websWrite(wp, T("GreenAPStart4: %s %s<br>\n"), sminute4, shour4);
	websWrite(wp, T("GreenAPEnd4: %s %s<br>\n"), eminute4, ehour4);
	websWrite(wp, T("GreenAPAction4: %s<br>\n"), action4);
	websFooter(wp);
	websDone(wp, 200);        
}
#endif

#ifdef CONFIG_USER_INADYN
/*
 * goform/DDNS
 */
static void DDNS(webs_t wp, char_t *path, char_t *query)
{
	char *ddns_provider, *ddns, *ddns_acc, *ddns_pass;
	char empty_char = '\0';

	ddns_provider = websGetVar(wp, T("DDNSProvider"), T("none"));
	ddns = websGetVar(wp, T("DDNS"), T(""));
	ddns_acc = websGetVar(wp, T("Account"), T(""));
	ddns_pass = websGetVar(wp, T("Password"), T(""));

	if(!ddns_provider || !ddns || !ddns_acc || !ddns_pass)
		return;

	if(!strcmp(T("none"), ddns_provider )){
		ddns = ddns_acc = ddns_pass = &empty_char;
	}else{
		if(!strlen(ddns) || !strlen(ddns_acc) || !strlen(ddns_pass))
			return;
	}

	if(checkSemicolon(ddns) || checkSemicolon(ddns_acc) || checkSemicolon(ddns_pass))
		return;

	nvram_bufset(RT2860_NVRAM, "DDNSProvider", ddns_provider);
	nvram_bufset(RT2860_NVRAM, "DDNS", ddns);
	nvram_bufset(RT2860_NVRAM, "DDNSAccount", ddns_acc);
	nvram_bufset(RT2860_NVRAM, "DDNSPassword", ddns_pass);
	nvram_commit(RT2860_NVRAM);

	doSystem("ddns.sh");

	websHeader(wp);
	websWrite(wp, T("<h2>DDNS Settings</h2><br>\n"));
	websWrite(wp, T("DDNSProvider: %s<br>\n"), ddns_provider);
	websWrite(wp, T("DDNS: %s<br>\n"), ddns);
	websWrite(wp, T("DDNSAccount: %s<br>\n"), ddns_acc);
	websWrite(wp, T("DDNSPassword: %s<br>\n"), ddns_pass);
	websFooter(wp);
	websDone(wp, 200);        
}
#endif

static void SystemCommand(webs_t wp, char_t *path, char_t *query)
{
	char *command;

	command = websGetVar(wp, T("command"), T(""));

	if(!command)
		return;

	if(!strlen(command))
		snprintf(system_command, COMMAND_MAX, "cat /dev/null > %s", SYSTEM_COMMAND_LOG);
	else
		snprintf(system_command, COMMAND_MAX, "%s 1>%s 2>&1", command, SYSTEM_COMMAND_LOG);
	
	if(strlen(system_command))
		doSystem(system_command);

	websRedirect(wp, "adm/system_command.asp");

	return;
}

static void repeatLastSystemCommand(webs_t wp, char_t *path, char_t *query)
{
	if(strlen(system_command))
		doSystem(system_command);

	websRedirect(wp, "adm/system_command.asp");

	return;
}


int showSystemCommandASP(int eid, webs_t wp, int argc, char_t **argv)
{
	FILE *fp;
	char buf[1024];
	
	fp = fopen(SYSTEM_COMMAND_LOG, "r");
	if(!fp){
		websWrite(wp, T(""));
		return 0;
	}

	while(fgets(buf, 1024, fp)){
		websWrite(wp, T("%s"), buf);
	}
	fclose(fp);
	
	return 0;
}

static inline char *strip_space(char *str)
{
	while( *str == ' ')
		str++;
	return str;
}


char* getField(char *a_line, char *delim, int count)
{
	int i=0;
	char *tok;
	tok = strtok(a_line, delim);
	while(tok){
		if(i == count)
			break;
        i++;
		tok = strtok(NULL, delim);
    }
    if(tok && isdigit(*tok))
		return tok;

	return NULL;
}

/*
 *   C version. (ASP version is below)
 */
static long long getIfStatistic(char *interface, int type)
{
	int found_flag = 0;
	int skip_line = 2;
	char buf[1024], *field, *semiColon = NULL;
	FILE *fp = fopen(PROC_IF_STATISTIC, "r");
	if(!fp){
		printf("no proc?\n");
		return -1;
	}

	while(fgets(buf, 1024, fp)){
		char *ifname;
		if(skip_line != 0){
			skip_line--;
			continue;
		}
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';
		ifname = buf;
		ifname = strip_space(ifname);

		if(!strcmp(ifname, interface)){
			found_flag = 1;
			break;
		}
	}
	fclose(fp);

	semiColon++;

	switch(type){
	case TXBYTE:
		if(  (field = getField(semiColon, " ", 8))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case TXPACKET:
		if(  (field = getField(semiColon, " ", 9))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case RXBYTE:
		if(  (field = getField(semiColon, " ", 0))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	case RXPACKET:
		if(  (field = getField(semiColon, " ", 1))  ){
			return strtoll(field, NULL, 10);
		}
		break;
	}
	return -1;
}

/*
 *     getIfStatistic()   ASP version
 */
int getIfStatisticASP(int eid, webs_t wp, int argc, char_t **argv)
{
	int found_flag = 0;
	int skip_line = 2;
	char *interface, *type, *field, *semiColon = NULL;
	char buf[1024], result[32];
	FILE *fp = fopen(PROC_IF_STATISTIC, "r");
	if(!fp){
		websWrite(wp, T("no proc?\n"));
		return -1;
	}

    if(ejArgs(argc, argv, T("%s %s"), &interface, &type) != 2){
		websWrite(wp, T("Wrong argument.\n"));
        return -1;
    }

	while(fgets(buf, 1024, fp)){
		char *ifname;
		if(skip_line != 0){
			skip_line--;
			continue;
		}
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';
		ifname = buf;
		ifname = strip_space(ifname);

		if(!strcmp(ifname, interface)){
			found_flag = 1;
			break;
		}
	}
	fclose(fp);

	semiColon++;

	if(!strcmp(type, T("TXBYTE")  )){
		if(  (field = getField(semiColon, " ", 8))  ){
			snprintf(result, 32,"%lld",   strtoll(field, NULL, 10));
			ejSetResult(eid, result);
		}
	}else if(!strcmp(type, T("TXPACKET")  )){
		if(  (field = getField(semiColon, " ", 9))  ){
			snprintf(result, 32,"%lld",   strtoll(field, NULL, 10));
			ejSetResult(eid, result);
		}
    }else if(!strcmp(type, T("RXBYTE")  )){
		if(  (field = getField(semiColon, " ", 0))  ){
			snprintf(result, 32,"%lld",   strtoll(field, NULL, 10));
			ejSetResult(eid, result);
		}
    }else if(!strcmp(type, T("RXPACKET")  )){
		if(  (field = getField(semiColon, " ", 1))  ){
			snprintf(result, 32,"%lld",   strtoll(field, NULL, 10));
			ejSetResult(eid, result);
		}
    }else{
		websWrite(wp, T("unknown type.") );
		return -1;
	}
	return -1;
}

int getWANRxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getWanIfName(), RXBYTE);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getWANRxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getWanIfName(), RXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getWANTxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getWanIfName(), TXBYTE);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getWANTxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getWanIfName(), TXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getLANRxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), RXBYTE);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getLANRxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), RXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getLANTxByteASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), TXBYTE);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"), buf);
	return 0;
}

int getLANTxPacketASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[32];
	long long data = getIfStatistic( getLanIfName(), TXPACKET);
	snprintf(buf, 32, "%lld", data);	
	websWrite(wp, T("%s"),buf);
	return 0;
}

/*
 * This ASP function is for javascript usage, ex:
 *
 * <script type="text/javascript">
 *   var a = new Array();
 *   a = [<% getAllNICStatisticASP(); %>];         //ex: a = ["lo","10","1000", "20", "2000","eth2"];
 *   document.write(a)
 * </script>
 *
 * Javascript could get info with  getAllNICStatisticASP().
 *
 * We dont produce table-related tag in this ASP function .It's
 * more extensive since ASP just handle data and Javascript present them,
 * although the data form is only for Javascript now.
 *
 * TODO: a lot, there are many ASP functions binding with table-relted tag...
 */
int getAllNICStatisticASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char result[1024];
	char buf[1024];
	int rc = 0, pos = 0, skip_line = 2;
	int first_time_flag = 1;
	FILE *fp = fopen(PROC_IF_STATISTIC, "r");
	if(!fp){
		printf("no proc?\n");
		return -1;
	}

	while(fgets(buf, 1024, fp)){
		char *ifname, *semiColon;
		long long if_rc;
		if(skip_line != 0){
			skip_line--;
			continue;
		}
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';

		ifname = buf;
		ifname = strip_space(ifname);

		/* try to get statistics data */
		if(getIfStatistic(ifname, RXPACKET) >= 0){
			/* a success try */
			if(first_time_flag){
				pos = snprintf(result+rc, 1024-rc, "\"%s\"", ifname);
				rc += pos;
				first_time_flag = 0;
			}else{
				pos = snprintf(result+rc, 1024-rc, ",\"%s\"", ifname);
				rc += pos;
			}

		}else	/* failed and just skip */
			continue;

		pos = snprintf(result+rc, 1024-rc, ",\"%lld\"", getIfStatistic(ifname, RXPACKET));
		rc += pos;
		pos = snprintf(result+rc, 1024-rc, ",\"%lld\"", getIfStatistic(ifname, RXBYTE));
		rc += pos;
		pos = snprintf(result+rc, 1024-rc, ",\"%lld\"", getIfStatistic(ifname, TXPACKET));
		rc += pos;
		pos = snprintf(result+rc, 1024-rc, ",\"%lld\"", getIfStatistic(ifname, TXBYTE));
		rc += pos;
	}
	fclose(fp);

	websWrite(wp, T("%s"), result);
    return 0;
}


int getMemTotalASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024], *semiColon, *key, *value;
	FILE *fp = fopen(PROC_MEM_STATISTIC, "r");
	if(!fp){
		websWrite(wp, T("no proc?\n"));
		return -1;
	}

	while(fgets(buf, 1024, fp)){
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';
		key = buf;
		value = semiColon + 1;
		if(!strcmp(key, "MemTotal")){
			value = strip_space(value);
			websWrite(wp, T("%s"), value);
			fclose(fp);
			return 0;
		}
	}
	websWrite(wp, T(""));
	fclose(fp);
	
	return -1;
}

int getCurrentTimeASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char_t buf[64];
	FILE *fp = popen("date", "r");
	if(!fp){
		websWrite(wp, T("none"));
		return 0;
	}
	fgets(buf, 64, fp);
	pclose(fp);

	websWrite(wp, T("%s"), buf);
	return 0;
}

int getMemLeftASP(int eid, webs_t wp, int argc, char_t **argv)
{
	char buf[1024], *semiColon, *key, *value;
	FILE *fp = fopen(PROC_MEM_STATISTIC, "r");
	if(!fp){
		websWrite(wp, T("no proc?\n"));
		return -1;
	}

	while(fgets(buf, 1024, fp)){
		if(! (semiColon = strchr(buf, ':'))  )
			continue;
		*semiColon = '\0';
		key = buf;
		value = semiColon + 1;
		if(!strcmp(key, "MemFree")){
			value = strip_space(value);
			websWrite(wp, T("%s"), value);
			fclose(fp);
			return 0;
		}
	}
	websWrite(wp, T(""));
	fclose(fp);
	return -1;
}

static int FirmwareUpgradePostASP(int eid, webs_t wp, int argc, char_t **argv)
{
#if 0
	FILE *fp;
	char ver[128], week[32], mon[32] , date[32], time[32], *pos;
	
	char *expect = nvram_bufget(RT2860_NVRAM, "Expect_Firmware");
	if(!expect || !strlen(expect) )
		return 0;

	fp = fopen("/proc/version", "r");
	if(!fp)
		return 0;

	fgets(ver, 128, fp);
	fclose(fp);	

	if(!(pos = strchr(ver, '#')) )
		return 0;

	if(!(pos = strchr(pos+1, ' ')) )
		return 0;

	pos++;
	sscanf(pos, "%s %s %s %s", week, mon, date, time);
	sprintf(ver, "Linux Kernel Image %s%s%s", mon, date, time); 

	if(!strcmp(expect, ver)){
		websWrite(wp, T("alert(\"Firmware Upgrade Success.\");"));
		nvram_bufset(RT2860_NVRAM, "Expect_Firmware", "");
		nvram_commit(RT2860_NVRAM);
	}else{
		websWrite(wp, T("alert(\"Firmware Upgrade may be failed:\\nexpect new image : %s\\ncurrent : %s\");"), expect, ver);
	}	
	return 0;
#endif
	FILE *fp;
	char buf[512];
	char *old_firmware = nvram_bufget(RT2860_NVRAM, "old_firmware");
	if(!old_firmware || !strlen(old_firmware) )
		return 0;
	fp = fopen("/proc/version", "r");
	if(!fp)
		return 0;

	fgets(buf, sizeof(buf), fp);
	fclose(fp);	
	if(!strcmp(buf, old_firmware)){
		websWrite(wp, T("alert(\"Warning!The firmware didn't change.\");"));
	}else{
		websWrite(wp, T("alert(\"Firmware Upgrade success\");"));
	}	
	nvram_bufset(RT2860_NVRAM, "old_firmware", "");
	nvram_commit(RT2860_NVRAM);

	return 0;
}

static void LoadDefaultSettings(webs_t wp, char_t *path, char_t *query)
{
	system("ralink_init clear 2860");
#if defined CONFIG_LAN_WAN_SUPPORT || defined CONFIG_MAC_TO_MAC_MODE
        system("ralink_init renew 2860 /etc_ro/Wireless/RT2860AP/RT2860_default_vlan");
#elif defined(CONFIG_ICPLUS_PHY)
        system("ralink_init renew 2860 /etc_ro/Wireless/RT2860AP/RT2860_default_oneport");
#else
        system("ralink_init renew 2860 /etc_ro/Wireless/RT2860AP/RT2860_default_novlan");
#endif
#if defined (CONFIG_INIC_MII) || defined (CONFIG_INIC_USB) || defined (CONFIG_INIC_PCI)
	system("ralink_init clear inic");
#if defined CONFIG_LAN_WAN_SUPPORT || defined CONFIG_MAC_TO_MAC_MODE
        system("ralink_init renew inic /etc_ro/Wireless/RT2860AP/RT2860_default_vlan");
#elif defined(CONFIG_ICPLUS_PHY)
        system("ralink_init renew inic /etc_ro/Wireless/RT2860AP/RT2860_default_oneport");
#else
        system("ralink_init renew inic /etc_ro/Wireless/RT2860AP/RT2860_default_novlan");
#endif
#endif
#if defined (CONFIG_RT2561_AP) || defined (CONFIG_RT2561_AP_MODULE)
	system("ralink_init clear 2561");
        system("ralink_init renew 2561 /etc_ro/Wireless/RT61AP/RT2561_default");
#endif
	system("reboot");
}


/*
 * callee must free memory.
 */
/*
static char *getLog(char *filename)
{
	FILE *fp;
	struct stat filestat;
	char *log;

	if(stat(filename, &filestat) == -1)
		return NULL;

//	printf("%d\n", filestat.st_size);
	log = (char *)malloc(sizeof(char) * (filestat.st_size + 1) );
	if(!log)
		return NULL;

	if(!(fp = fopen(filename, "r"))){
		return NULL;
	}

	if( fread(log, 1, filestat.st_size, fp) != filestat.st_size){
		printf("read not enough\n");
		free(log);
		return NULL;
	}

	log[filestat.st_size] = '\0';

	fclose(fp);
	return log;
}
*/

#if defined CONFIG_LOGREAD && defined CONFIG_KLOGD
static void clearlog(webs_t wp, char_t *path, char_t *query)
{
	doSystem("killall -q klogd");
	doSystem("killall -q syslogd");
	doSystem("syslogd -S -C8 1>/dev/null 2>&1");
	doSystem("klogd 1>/dev/null 2>&1");

	websRedirect(wp, "adm/syslog.asp");
}
#endif

#define LOG_MAX (32768)
//#define LOG_MAX (16384)
static void syslog(webs_t wp, char_t *path, char_t *query)
{
	FILE *fp = NULL;
	char_t *log;
	char buf[300];

	websWrite(wp, T("HTTP/1.1 200 OK\nContent-type: text/plain\nPragma: no-cache\nCache-Control: no-cache\n\n"));

	fp = popen("logread", "r");
	if(!fp){
		websWrite(wp, "-1");
		goto error;
	}
/*
	log = malloc(LOG_MAX * sizeof(char));
	if(!log){
		websWrite(wp, "-1");
		goto error;
	}

	memset(log, 0, LOG_MAX);
	fread(log, 1, LOG_MAX, fp);
	websLongWrite(wp, log);
*/
	while(1)
	{
		if(fgets(buf,sizeof(buf),fp)==NULL) break;
		websWrite(wp, buf);
	}


	//free(log);
error:
	if(fp)
		pclose(fp);
	websDone(wp, 200);
}

void management_init(void)
{
	doSystem("ntp.sh");
#ifdef CONFIG_USER_GOAHEAD_GreenAP
    	doSystem("greenap.sh init");
#endif
	doSystem("ddns.sh");
	WPSRestart();

	doSystem("killall -q klogd");
	doSystem("killall -q syslogd");
	doSystem("syslogd -S -C8 1>/dev/null 2>&1");
	doSystem("klogd 1>/dev/null 2>&1");
}

void management_fini(void)
{
	doSystem("killall -q klogd");
	doSystem("killall -q syslogd");
}

static int getGAPBuilt(int eid, webs_t wp, int argc, char_t **argv)
{
#ifdef CONFIG_USER_GOAHEAD_GreenAP
	return websWrite(wp, T("1"));
#else
	return websWrite(wp, T("0"));
#endif
}


static void title_bar_select(int eid, webs_t wp, int argc, char_t **argv)
{
        char* title_bar_str=nvram_get(RT2860_NVRAM,"TITLE_BAR_STR");
        websWrite(wp, T("<img src=\"graphics/%s\" width=\"728\" height=\"60\" border=\"0\">\n"),title_bar_str);

}





void formDefineManagement(void)
{
	websFormDefine(T("setSysAdm"), setSysAdm);
	websFormDefine(T("setSysLang"), setSysLang);
	websFormDefine(T("NTP"), NTP);
#ifdef CONFIG_DATE
	websFormDefine(T("NTPSyncWithHost"), NTPSyncWithHost);
#endif
	websAspDefine(T("getCurrentTimeASP"), getCurrentTimeASP);
	websAspDefine(T("getGAPBuilt"), getGAPBuilt);
#ifdef CONFIG_USER_GOAHEAD_GreenAP
	websFormDefine(T("GreenAP"), GreenAP);
#endif
#ifdef CONFIG_USER_INADYN
	websFormDefine(T("DDNS"), DDNS);
#endif

	websAspDefine(T("getMemLeftASP"), getMemLeftASP);
	websAspDefine(T("getMemTotalASP"), getMemTotalASP);

	websAspDefine(T("getWANRxByteASP"), getWANRxByteASP);
	websAspDefine(T("getWANTxByteASP"), getWANTxByteASP);
	websAspDefine(T("getLANRxByteASP"), getLANRxByteASP);
	websAspDefine(T("getLANTxByteASP"), getLANTxByteASP);
	websAspDefine(T("getWANRxPacketASP"), getWANRxPacketASP);
	websAspDefine(T("getWANTxPacketASP"), getWANTxPacketASP);
	websAspDefine(T("getLANRxPacketASP"), getLANRxPacketASP);
	websAspDefine(T("getLANTxPacketASP"), getLANTxPacketASP);

	websAspDefine(T("getAllNICStatisticASP"), getAllNICStatisticASP);

	websAspDefine(T("showSystemCommandASP"), showSystemCommandASP);
	websFormDefine(T("SystemCommand"), SystemCommand);
	websFormDefine(T("repeatLastSystemCommand"), repeatLastSystemCommand);

	websFormDefine(T("LoadDefaultSettings"), LoadDefaultSettings);

	websFormDefine(T("syslog"), syslog);
#if defined CONFIG_LOGREAD && defined CONFIG_KLOGD
	websFormDefine(T("clearlog"), clearlog);
#endif

	websAspDefine(T("FirmwareUpgradePostASP"), FirmwareUpgradePostASP);
   websAspDefine(T("title_bar_select"), title_bar_select);


	formDefineWPS();
}
