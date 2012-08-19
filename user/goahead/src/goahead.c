/* vi: set sw=4 ts=4 sts=4: */
/*
 * main.c -- Main program for the GoAhead WebServer (LINUX version)
 *
 * Copyright (c) GoAhead Software Inc., 1995-2000. All Rights Reserved.
 *
 * See the file "license.txt" for usage and redistribution license requirements
 *
 * $Id: goahead.c,v 1.100.2.4 2009-04-08 08:52:59 chhung Exp $
 */

/******************************** Description *********************************/

/*
 *	Main program for for the GoAhead WebServer. This is a demonstration
 *	main program to initialize and configure the web server.
 */

/********************************* Includes ***********************************/

#include	"uemf.h"
#include	"wsIntrn.h"
#include	"nvram.h"
#include	"ralink_gpio.h"
#include	"internet.h"
#if defined INIC_SUPPORT || defined INICv2_SUPPORT
#include	"inic.h"
#endif
#if defined (CONFIG_RT2561_AP) || defined (CONFIG_RT2561_AP_MODULE)
#include	"legacy.h"
#endif
#include	"utils.h"
#include	"wireless.h"
#include	"firewall.h" 
#include	"management.h"
#include	"station.h"
#include	"usb.h"
#include	"media.h"
#include	<signal.h>
#include	<unistd.h> 
#include	<sys/types.h>
#include	<sys/wait.h>
#include	"linux/autoconf.h"
#include	"config/autoconf.h" //user config
#include    <pthread.h>



#ifdef CONFIG_RALINKAPP_SWQOS
#include      "qos.h"
#endif

#ifdef WEBS_SSL_SUPPORT
#include	"websSSL.h"
#endif


#include "vpn.h"
#include "dtu_action.h"
#include "msg_action.h"
#include "sr_action.h"
#include "linkbackup_action.h"
#include "reboot_action.h"
#include "pptp_action.h"
#include "l2tp_action.h"
#include "vrrp_action.h"
#include "gps_action.h"
#include "snmp_action.h"
#ifdef USER_MANAGEMENT_SUPPORT
#include	"um.h"
void	formDefineUserMgmt(void);
#endif



/*********************************** Locals ***********************************/
/*
 *	Change configuration here
 */

static char_t		*rootWeb = T("/etc_ro/web");		/* Root web directory */
static char_t		*password = T("");				/* Security password */
static int			port = 80;						/* Server port */
static int			retries = 5;					/* Server port retries */
static int			finished;						/* Finished flag */
static char_t		*gopid = T("/var/run/goahead.pid");	/* pid file */

/****************************** Forward Declarations **************************/

static int	writeGoPid(void);
static int 	initSystem(void);
static int 	initWebs(void);
static int  websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
				int arg, char_t *url, char_t *path, char_t *query);
extern void defaultErrorHandler(int etype, char_t *msg);
extern void defaultTraceHandler(int level, char_t *buf);
extern void ripdRestart(void);
#ifdef B_STATS
static void printMemStats(int handle, char_t *fmt, ...);
static void memLeaks();
#endif
extern void WPSAPPBCStartAll(void);
extern void WPSSingleTriggerHandler(int);
#if defined CONFIG_USB
extern void hotPluglerHandler(int);
#endif


#ifdef CONFIG_RT2860V2_STA_WSC
extern void WPSSTAPBCStartEnr(void);
#endif

#ifdef CONFIG_DUAL_IMAGE
static int set_stable_flag(void);
#endif

void initDeviceName()
{
	char*pdev;
	char *buf;
	char szBuf[128];

    pdev = nvram_bufget(RT2860_NVRAM, "wan_3g_dev"); 
	if((strcmp(pdev, "HUAWEI-EM560") == 0) || (strcmp(pdev, "F3607gw") == 0))
	{
	//	stream=popen("3GInfo -d /dev/ttyACM2 -s","r");
		//stream=popen("comgt -d /dev/ttyACM2 -s /etc_ro/ppp/3g/signal.scr","r");
		buf = "/dev/ttyACM2";
	}
	else if(strcmp(pdev, "HUAWEI-EM660") == 0)
	{
		buf = "/dev/ttyUSB2";
	}
	else if(strcmp(pdev, "IE901D") == 0)
        {
               buf = "/dev/ttyUSB1";
        }

	else if(strcmp(pdev, "HUAWEI-EM770") == 0)
	{
		buf = "/dev/ttyUSB2";
	}
	else if(strcmp(pdev,"THINKWILL-MI600")==0)
	{
		//stream=popen("comgt -d /dev/ttyUSB4 -s /etc_ro/ppp/3g/signal.scr","r");
		buf = "/dev/ttyUSB4"; //-m signal range
	}
	else if(strcmp(pdev,"SYNCWISER-801/401")==0)
	{
		buf = "/dev/ttyUSB2"; //-m signal range
	}
	else if(strcmp(pdev,"LONGSUNG-C5300")==0)
	{
		buf = "/dev/ttyUSB3"; //-m signal range
	}
	else if(strcmp(pdev,"LONGSUNG-U6300/U5300")==0)
	{
		buf = "/dev/ttyUSB1"; //-m signal range
	}
	else if(strcmp(pdev,"GAORAN-280")==0)
	{
		buf = "/dev/ttyUSB3"; //-m signal range
	}
	else if(strcmp(pdev,"TW-W1M100")==0)
	{
		buf = "/dev/ttyUSB1"; //-m signal range
	}
	else if(strcmp(pdev,"ZTE-MU301")==0)
	{
		buf = "/dev/ttyUSB2"; //-m signal range
	}
	else if(strcmp(pdev,"ZTE-MF210V")==0)
	{
		//stream=popen("3GInfo -d /dev/ttyUSB2  -m 0-31 ","r"); //-m signal range MF210V
		buf = "/dev/ttyUSB1"; //-m signal range  MF210
	}
	else if(strcmp(pdev,"KSE-360")==0)
	{
		buf = "/dev/ttyUSB1"; //-m signal range
	}
	else if(strcmp(pdev,"ZX-600")==0)
	{
		buf = "/dev/ttyUSB2"; //-m signal range
	}
	
         else if(strcmp(pdev,"SIERRA-MC8785")==0)
        {
                buf = "/dev/ttyUSB6"; //-m signal range

        }
	 else if(strcmp(pdev,"AD3812")==0)
        {
                buf = "/dev/ttyUSB0"; //-m signal range

        }
	else 
	{
		buf = "/dev/ttyUSB2";
	}
	
	system("rm -f /dev/yh");
	sprintf(szBuf, "ln -s %s /dev/yh", buf);
	system(szBuf);
	sprintf(szBuf, "%s/mklink.sh", rootWeb);
	system(szBuf);

}

void check_vpn()
{
    static time_t last =0;
    time_t now;
    char *rules;

    time(&now);
    // check vpn every 30 secs
    if(now - last >= 30)
    {
        last = now;
    }
    else
    {
        return;
    }
    
    rules = nvram_bufget(RT2860_NVRAM, "IPSECRules");
    if(rules && (strstr(rules,"|1|") != NULL))
    {
        char if_addr[32];
        //@TODO: temp use ppp0 as the default 3g interface name
        if( getIfIp("ppp0", if_addr) != 0)
        {
            // 3g is down
            return;
        }
    }
    else
    {
        // ipsec is disabled
        return;
    }
    
    // check the ping_any.sh, if it is not started, start it
    if(system("ps>/var/check_thread.log && cat /var/check_thread.log|grep \"ping_any\"") != 0)//is not found.
    {
        system("ping_any.sh&"); 
    }
}

/*********************************** Code *************************************/
/*
 *	Main -- entry point from LINUX
 */
void InitMfgTask();
int main(int argc, char** argv)
{
    int wdt_fd = -1;
    
    wdt_fd = open("/dev/watchdog", O_WRONLY);
    if (wdt_fd == -1)
    {
        // fail to open watchdog device
        printf("can not open watchdog!!!!!!!!!!!!!!1\n");
        exit(1);
    }

/*
 *	Initialize the memory allocator. Allow use of malloc and start 
 *	with a 60K heap.  For each page request approx 8KB is allocated.
 *	60KB allows for several concurrent page requests.  If more space
 *	is required, malloc will be used for the overflow.
 */
	bopen(NULL, (60 * 1024), B_USE_MALLOC);
	signal(SIGPIPE, SIG_IGN);

	if (writeGoPid() < 0)
		return -1;
	if (initSystem() < 0)
		return -1;

	initDeviceName();
	
/*
 *	Initialize the web server
 */
	if (initWebs() < 0) {
		return -1;
	}

#ifdef CONFIG_DUAL_IMAGE
/* Set stable flag after the web server is started */
	set_stable_flag();
#endif

#ifdef WEBS_SSL_SUPPORT
	websSSLOpen();
#endif

/*
 *	Basic event loop. SocketReady returns true when a socket is ready for
 *	service. SocketSelect will block until an event occurs. SocketProcess
 *	will actually do the servicing.
 */
 	InitMfgTask();
	while (!finished) {
		if (socketReady(-1) || socketSelect(-1, 1000)) {
			socketProcess(-1);
		}
        if (wdt_fd != -1)
            write(wdt_fd, "a", 1);
       
        check_vpn();
		websCgiCleanup();
		emfSchedProcess();
	}

#ifdef WEBS_SSL_SUPPORT
	websSSLClose();
#endif

#ifdef USER_MANAGEMENT_SUPPORT
	umClose();
#endif

/*
 *	Close the socket module, report memory leaks and close the memory allocator
 */
	websCloseServer();
	socketClose();
#ifdef B_STATS
	memLeaks();
#endif
	bclose();
	return 0;
}

/******************************************************************************/
/*
 *	Write pid to the pid file
 */
int writeGoPid(void)
{
	FILE *fp;

	fp = fopen(gopid, "w+");
	if (NULL == fp) {
		error(E_L, E_LOG, T("goahead.c: cannot open pid file"));
		return (-1);
	}
	fprintf(fp, "%d", getpid());
    fclose(fp);
	return 0;
}

static void goaSigHandler(int signum)
{
#ifdef CONFIG_RT2860V2_STA_WSC
	char *opmode = nvram_bufget(RT2860_NVRAM, "OperationMode");
	char *ethCon = nvram_bufget(RT2860_NVRAM, "ethConvert");
#endif

	if (signum != SIGUSR1)
		return;

#ifdef CONFIG_RT2860V2_STA_WSC
	if(!strcmp(opmode, "2") || (!strcmp(opmode, "0") &&   !strcmp(ethCon, "1") ) )		// wireless isp mode
		WPSSTAPBCStartEnr();	// STA WPS default is "Enrollee mode".
	else
#endif
		WPSAPPBCStartAll();
}

#ifndef CONFIG_RALINK_RT2880
static void goaInitGpio()
{
	int fd;
	ralink_gpio_reg_info info;

	fd = open("/dev/gpio", O_RDONLY);
	if (fd < 0) {
		perror("/dev/gpio");
		return;
	}
	//set gpio direction to input
	if (ioctl(fd, RALINK_GPIO_SET_DIR_IN, RALINK_GPIO(0)) < 0)
		goto ioctl_err;
	//enable gpio interrupt
	if (ioctl(fd, RALINK_GPIO_ENABLE_INTP) < 0)
		goto ioctl_err;
	//register my information
	info.pid = getpid();
	info.irq = 0;
	if (ioctl(fd, RALINK_GPIO_REG_IRQ, &info) < 0)
		goto ioctl_err;
	close(fd);

	//issue a handler to handle SIGUSR1
	signal(SIGUSR1, goaSigHandler);
	return;

ioctl_err:
	perror("ioctl");
	close(fd);
	return;
}
#endif

static void dhcpcHandler(int signum)
{
	ripdRestart();
}


/******************************************************************************/
/*
 *	Initialize System Parameters
 */
static int initSystem(void)
{
	int setDefault(void);

	signal(SIGUSR2, dhcpcHandler);
	if (setDefault() < 0)
		return (-1);
	if (initInternet() < 0)
		return (-1);
#if defined CONFIG_USB
	signal(SIGTTIN, hotPluglerHandler);
	hotPluglerHandler(SIGTTIN);
#endif
#ifdef CONFIG_RALINK_RT2880
	signal(SIGUSR1, goaSigHandler);
#else
	//goaInitGpio();
	signal(SIGUSR1, goaSigHandler); //receive wpsledpbc SIGUSR1  from wps key
#endif
	signal(SIGXFSZ, WPSSingleTriggerHandler);

	return 0;
}

/******************************************************************************/
/*
 *	Set Default should be done by nvram_daemon.
 *	We check the pid file's existence.
 */
int setDefault(void)
{
	FILE *fp;
	int i;

	//retry 15 times (15 seconds)
	for (i = 0; i < 15; i++) {
		fp = fopen("/var/run/nvramd.pid", "r");
		if (fp == NULL) {
			if (i == 0)
				trace(0, T("goahead: waiting for nvram_daemon "));
			else
				trace(0, T(". "));
		}
		else {
			fclose(fp);
			nvram_init(RT2860_NVRAM);
#if defined INIC_SUPPORT || defined INICv2_SUPPORT
			nvram_init(RTINIC_NVRAM);
#endif
#if defined (CONFIG_RT2561_AP) || defined (CONFIG_RT2561_AP_MODULE)
			nvram_init(RT2561_NVRAM);
#endif
			return 0;
		}
		Sleep(1);
	}
	error(E_L, E_LOG, T("goahead: please execute nvram_daemon first!"));
	return (-1);
}

/******************************************************************************/
/*
 *	Initialize the web server.
 */

static int initWebs(void)
{
	struct in_addr	intaddr;
#ifdef GA_HOSTNAME_SUPPORT
	struct hostent	*hp;
	char			host[128];
#else
	char			*lan_ip = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
#endif
	char			webdir[128];
	char			*cp;
	char_t			wbuf[128];

/*
 *	Initialize the socket subsystem
 */
	socketOpen();

#ifdef USER_MANAGEMENT_SUPPORT
/*
 *	Initialize the User Management database
 */
	char *admu = nvram_bufget(RT2860_NVRAM, "Login");
	char *admp = nvram_bufget(RT2860_NVRAM, "Password");
	umOpen();
	//umRestore(T("umconfig.txt"));
	//winfred: instead of using umconfig.txt, we create 'the one' adm defined in nvram
	umAddGroup(T("adm"), 0x07, AM_DIGEST, FALSE, FALSE);
	if (admu && strcmp(admu, "") && admp && strcmp(admp, "")) {
		umAddUser(admu, admp, T("adm"), FALSE, FALSE);
		umAddAccessLimit(T("/"), AM_DIGEST, FALSE, T("adm"));
	}
	else
		error(E_L, E_LOG, T("gohead.c: Warning: empty administrator account or password"));
#endif

#ifdef GA_HOSTNAME_SUPPORT
/*
 *	Define the local Ip address, host name, default home page and the 
 *	root web directory.
 */
	if (gethostname(host, sizeof(host)) < 0) {
		error(E_L, E_LOG, T("gohead.c: Can't get hostname"));
		return -1;
	}
	if ((hp = gethostbyname(host)) == NULL) {
		error(E_L, E_LOG, T("gohead.c: Can't get host address"));
		return -1;
	}
	memcpy((char *) &intaddr, (char *) hp->h_addr_list[0],
		(size_t) hp->h_length);
#else
/*
 * get ip address from nvram configuration (we executed initInternet)
 */
	if (NULL == lan_ip) {
		error(E_L, E_LOG, T("initWebs: cannot find lan_ip in NVRAM"));
		return -1;
	}
	intaddr.s_addr = inet_addr("0.0.0.0");
	if (intaddr.s_addr == INADDR_NONE) {
		error(E_L, E_LOG, T("initWebs: failed to convert %s to binary ip data"),
				lan_ip);
		return -1;
	}
#endif

/*
 *	Set rootWeb as the root web. Modify this to suit your needs
 */
	sprintf(webdir, "%s", rootWeb);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultDir(webdir);
	cp = inet_ntoa(intaddr);
	ascToUni(wbuf, cp, min(strlen(cp) + 1, sizeof(wbuf)));
	websSetIpaddr(wbuf);
#ifdef GA_HOSTNAME_SUPPORT
	ascToUni(wbuf, host, min(strlen(host) + 1, sizeof(wbuf)));
#else
	//use ip address (already in wbuf) as host
#endif
	websSetHost(wbuf);

/*
 *	Configure the web server options before opening the web server
 */
	websSetDefaultPage(T("default.asp"));
	websSetPassword(password);

/* 
 *	Open the web server on the given port. If that port is taken, try
 *	the next sequential port for up to "retries" attempts.
 */
	websOpenServer(port, retries);

/*
 * 	First create the URL handlers. Note: handlers are called in sorted order
 *	with the longest path handler examined first. Here we define the security 
 *	handler, forms handler and the default web page handler.
 */
	websUrlHandlerDefine(T(""), NULL, 0, websSecurityHandler, 
		WEBS_HANDLER_FIRST);
	websUrlHandlerDefine(T("/goform"), NULL, 0, websFormHandler, 0);
	websUrlHandlerDefine(T("/cgi-bin"), NULL, 0, websCgiHandler, 0);
	websUrlHandlerDefine(T(""), NULL, 0, websDefaultHandler, 
		WEBS_HANDLER_LAST); 

/*
 *	Define our functions
 */
	formDefineUtilities();
	formDefineInternet();
#if defined CONFIG_RALINKAPP_SWQOS
	formDefineQoS();
#endif
#if defined CONFIG_USB
	formDefineUSB();
#endif
#if defined CONFIG_RALINKAPP_MPLAYER
	formDefineMedia();
#endif
	formDefineWireless();
#if defined INIC_SUPPORT || defined INICv2_SUPPORT
	formDefineInic();
#endif
#if defined (CONFIG_RT2561_AP) || defined (CONFIG_RT2561_AP_MODULE)
	formDefineLegacy();
#endif
#if defined CONFIG_RT2860V2_STA || defined CONFIG_RT2860V2_STA_MODULE
	formDefineStation();
#endif
	formDefineFirewall();
	formDefineManagement();
  formDefineVPN();
  init_dtu();
  init_msg();
  init_status_report();
	init_linkbackup();
	init_reboot();
	init_pptp();
	init_l2tp();
	init_vrrp();
  init_gps();
  init_snmp();

/*
 *	Create the Form handlers for the User Management pages
 */
#ifdef USER_MANAGEMENT_SUPPORT
	//formDefineUserMgmt();  winfred: we do it ourselves
#endif

/*
 *	Create a handler for the default home page
 */
	websUrlHandlerDefine(T("/"), NULL, 0, websHomePageHandler, 0); 
	return 0;
}

/******************************************************************************/
/*
 *	Home page handler
 */

static int websHomePageHandler(webs_t wp, char_t *urlPrefix, char_t *webDir,
	int arg, char_t *url, char_t *path, char_t *query)
{
/*
 *	If the empty or "/" URL is invoked, redirect default URLs to the home page
 */
	if (*url == '\0' || gstrcmp(url, T("/")) == 0) {
		websRedirect(wp, T("home.asp"));
		return 1;
	}
	return 0;
}

/******************************************************************************/
/*
 *	Default error handler.  The developer should insert code to handle
 *	error messages in the desired manner.
 */

void defaultErrorHandler(int etype, char_t *msg)
{
	write(1, msg, gstrlen(msg));
}

/******************************************************************************/
/*
 *	Trace log. Customize this function to log trace output
 */

void defaultTraceHandler(int level, char_t *buf)
{
/*
 *	The following code would write all trace regardless of level
 *	to stdout.
 */
	if (buf) {
		if (0 == level)
			write(1, buf, gstrlen(buf));
	}
}

/******************************************************************************/
/*
 *	Returns a pointer to an allocated qualified unique temporary file name.
 *	This filename must eventually be deleted with bfree();
 */
#if defined CONFIG_USB_STORAGE && defined CONFIG_USER_STORAGE
char_t *websGetCgiCommName(webs_t wp)
{
	char *force_mem_upgrade = nvram_bufget(RT2860_NVRAM, "Force_mem_upgrade");
	char_t	*pname1 = NULL, *pname2 = NULL;
	char *part;

	if(!strcmp(force_mem_upgrade, "1")){
		pname1 = (char_t *)tempnam(T("/var"), T("cgi"));
	}else if(wp && (wp->flags & WEBS_CGI_FIRMWARE_UPLOAD) ){
		// see if usb disk is present and available space is enough?
		if( (part = isStorageOK()) )
			pname1 = (char_t *)tempnam(part, T("cgi"));
		else
			pname1 = (char_t *)tempnam(T("/var"), T("cgi"));
	}else{
		pname1 = (char_t *)tempnam(T("/var"), T("cgi"));
	}

	pname2 = bstrdup(B_L, pname1);
	free(pname1);

	return pname2;
}
#else
char_t *websGetCgiCommName(webs_t wp)
{
	char_t	*pname1, *pname2;

	pname1 = (char_t *)tempnam(T("/var"), T("cgi"));
	pname2 = bstrdup(B_L, pname1);
	free(pname1);

	return pname2;
}
#endif
/******************************************************************************/
/*
 *	Launch the CGI process and return a handle to it.
 */

int websLaunchCgiProc(char_t *cgiPath, char_t **argp, char_t **envp,
					  char_t *stdIn, char_t *stdOut)
{
	int	pid, fdin, fdout, hstdin, hstdout, rc;

	fdin = fdout = hstdin = hstdout = rc = -1; 
	if ((fdin = open(stdIn, O_RDWR | O_CREAT, 0666)) < 0 ||
		(fdout = open(stdOut, O_RDWR | O_CREAT, 0666)) < 0 ||
		(hstdin = dup(0)) == -1 ||
		(hstdout = dup(1)) == -1 ||
		dup2(fdin, 0) == -1 ||
		dup2(fdout, 1) == -1) {
		goto DONE;
	}

 	rc = pid = fork();
 	if (pid == 0) {
/*
 *		if pid == 0, then we are in the child process
 */
		if (execve(cgiPath, argp, envp) == -1) {
			printf("content-type: text/html\n\n"
				"Execution of cgi process failed\n");
		}
		exit (0);
	} 

DONE:
	if (hstdout >= 0) {
		dup2(hstdout, 1);
      close(hstdout);
	}
	if (hstdin >= 0) {
		dup2(hstdin, 0);
      close(hstdin);
	}
	if (fdout >= 0) {
		close(fdout);
	}
	if (fdin >= 0) {
		close(fdin);
	}
	return rc;
}

/******************************************************************************/
/*
 *	Check the CGI process.  Return 0 if it does not exist; non 0 if it does.
 */

int websCheckCgiProc(int handle, int *status)
{
/*
 *	Check to see if the CGI child process has terminated or not yet.  
 */
	if (waitpid(handle, status, WNOHANG) == handle) {
		return 0;
	} else {
		return 1;
	}
}

/******************************************************************************/

#ifdef B_STATS
static void memLeaks() 
{
	int		fd;

	if ((fd = gopen(T("leak.txt"), O_CREAT | O_TRUNC | O_WRONLY, 0666)) >= 0) {
		bstats(fd, printMemStats);
		close(fd);
	}
}

/******************************************************************************/
/*
 *	Print memory usage / leaks
 */

static void printMemStats(int handle, char_t *fmt, ...)
{
	va_list		args;
	char_t		buf[256];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	write(handle, buf, strlen(buf));
}
#endif

/******************************************************************************/

/* added by YYhuang 07/04/02 */
int getGoAHeadServerPort(void)
{
    return port;
}

#ifdef CONFIG_DUAL_IMAGE
static int set_stable_flag(void)
{
	int set = 0;
	char *wordlist = nvram_get(UBOOT_NVRAM, "Image1Stable");

	if (wordlist) {
		if (strcmp(wordlist, "1") != 0)
			set = 1;
	}
	else
		set = 1;

	if (set) {
		printf("Set Image1 stable flag\n");
		nvram_set(UBOOT_NVRAM, "Image1Stable", "1");
	}
	
	return 0;

}
#endif











int call_shell(char *cmdbuf,char *outbuf,int outBufLen)
{
	FILE *fp;
	int iLen = 0,lentmp;
	char bufTmp[256];

	fp = popen(cmdbuf,"r");
	if (fp == NULL) {
		return -1;
	}
	for (;;) {
		memset(bufTmp,0,sizeof(bufTmp));
		if (fgets(bufTmp,sizeof(bufTmp),fp) == NULL)
			break;
		lentmp = strlen(bufTmp);
		if ((iLen + lentmp + 1) > outBufLen)
			break;
		memcpy(outbuf + iLen,bufTmp,lentmp);
		iLen += lentmp;
	}

	pclose(fp);


	*(outbuf + iLen) = 0;

	return iLen;
}


#define RECV_MAX_LEN 128
#define SEND_MAX_LEN 2048
extern int readUsb(char *fileName);
void MfgThread()
{

	int mfg_fd;
	int i,iLen;
	struct sockaddr_in local,remote;
	char *LocalIP;
	char *pos;

	char RecvBuf[RECV_MAX_LEN],SendBuf[SEND_MAX_LEN];
	char FlagBuf[RECV_MAX_LEN],CmdBuf[RECV_MAX_LEN],ValBuf[RECV_MAX_LEN];

	memset( &local, 0, sizeof(local) );
	local.sin_family = AF_INET;
	local.sin_port = htons(24151);
	LocalIP = nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
	//GetCfmValue("lan_ipaddr", LocalIP);
	//local.sin_addr.s_addr  = INADDR_ANY;
	local.sin_addr.s_addr  = inet_addr(LocalIP);

	mfg_fd = socket( AF_INET,SOCK_DGRAM, 0 );
	if ( mfg_fd < 0 ) {
		printf("MfgThread socket error.\n");
		return ;
	}
	int n = 1;
	if(setsockopt(mfg_fd, SOL_SOCKET, SO_REUSEADDR, (char *) &n, sizeof(n)) == -1)
	{
		close(mfg_fd);
		printf("MfgThread: setsockopt 1 failed\n");
		return ;
	}
	if (bind(mfg_fd,(struct sockaddr*)&local,sizeof(local)) < 0) {
		printf("MfgThread bind error.\n");
		return;
	}
	while (1) {
		memset(RecvBuf,0,sizeof(RecvBuf));
		i = sizeof(struct sockaddr);
		iLen = recvfrom(mfg_fd,(char *)RecvBuf,RECV_MAX_LEN,0,(struct sockaddr*)&remote,&i);
		if (iLen <= 0) {

			sleep(1);
			continue;
		}
		//printf("MfgThread recv %d[%s]\n",iLen,RecvBuf);
		if (iLen < 14) {
			continue;
		}

		memset(FlagBuf,0,RECV_MAX_LEN);
		memset(CmdBuf,0,RECV_MAX_LEN);
		memset(ValBuf,0,RECV_MAX_LEN);
		//Request:
		//cmd fmt: w302r_mfg 1 cmd[...]
		//1:cmd ±ØÐëÎªiwprivÃüÁî
		// 
		// Response:
		// cmd fmt: result(cmd out stream)

		memcpy(FlagBuf,RecvBuf,9);
		memcpy(CmdBuf,RecvBuf + 10,1);
		memcpy(ValBuf,RecvBuf + 12,iLen - 12);

		//printf("[%s][%s][%s]\n",FlagBuf,CmdBuf,ValBuf);

		if (strcmp(FlagBuf,"rlink_mfg") != 0) {
			continue;
		}
		memset(SendBuf,0,SEND_MAX_LEN);
		if (CmdBuf[0] == '1') {
				pos = strstr(ValBuf,"iwpriv");
				if (pos == NULL) {
					continue;
				}
				//printf("Req[%s]\n",ValBuf);
				iLen = call_shell(ValBuf,SendBuf,SEND_MAX_LEN);
				if (iLen > 0){
					printf("Res[%s]\n",ValBuf);
					sendto(mfg_fd,(char *)SendBuf,iLen,0,(struct sockaddr*)&remote,sizeof(remote));
				} else {
					strcpy(SendBuf,"000000");
					iLen = strlen(SendBuf);
					printf("Res[%s]\n",SendBuf);
					sendto(mfg_fd,(char *)SendBuf,iLen,0,(struct sockaddr*)&remote,sizeof(remote));
				}
		} else if (CmdBuf[0] == 'x') {
			iLen = call_shell(ValBuf,SendBuf,SEND_MAX_LEN);
			//printf("*[%d][%s]\n",iLen,SendBuf);
			if (iLen > 0){
				sendto(mfg_fd,(char *)SendBuf,iLen,0,(struct sockaddr*)&remote,sizeof(remote));
			}
		} else if (CmdBuf[0] == 'e') {
			iLen = strlen("ralink_mfg");
			strcpy(SendBuf,"ralink_mfg");
			sendto(mfg_fd,(char *)SendBuf,iLen,0,(struct sockaddr*)&remote,sizeof(remote));
		}else if (CmdBuf[0] == 'u'){
			//if(readUsb(ValBuf)){
			//	strcpy(SendBuf,"USB Success.");
			//}else
			//	strcpy(SendBuf,"USB Failed.");
		
			//sendto(mfg_fd,(char *)SendBuf,iLen,0,(struct sockaddr*)&remote,sizeof(remote));
		}
	}
}


void InitMfgTask()
{

	pthread_t id;
	int ret;

	ret = pthread_create(&id, NULL,(void *) MfgThread,NULL);

	return ;
}
