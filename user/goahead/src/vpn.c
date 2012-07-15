/* vi: set sw=4 ts=4 sts=4 fdm=marker: */
/*
 *	vpn.c -- Settings  IPSEC vpn
 *
 *	Copyright (c) T&W Corporation All Rights Reserved.
 *
 *	$Id: vpv.c,v 1.167.2.8 2009-12 add by paul  $
 */

#include	<stdlib.h>
#include	<sys/ioctl.h>
#include	<net/if.h>
#include	<net/route.h>
#include    <string.h>
#include    <dirent.h>
//#include 	<winsock.h>

#include	"internet.h"
#include	"nvram.h"
#include	"webs.h"
#include	"utils.h"
#include 	"firewall.h"

#include	"linux/autoconf.h"  //kernel config
#include	"config/autoconf.h" //user config
#include	"user/busybox/include/autoconf.h" //busybox config

#define IPSEC_MAX_RULES  20	
#define CERT_PATH "/var/cert"

void ipsec_run(void);
static int vpn_edit_load_para(void);


	/* vpn handle. paul 200912.  */
static void setipsec_config(webs_t wp, char_t *path, char_t *query);

static int isIpValid(char *str)
{
	/*
	struct in_addr addr;	// for examination
	if( (! strcmp(T("any"), str)) || (! strcmp(T("any/0"), str)))
		return 1;

	if(! (inet_aton(str, &addr))){
		printf("isIpValid(): %s is not a valid IP address.\n", str);
		return 0;
	}
	*/
	return 1;
}

int rutIPSec_calPrefixLen(char *ipAddr)
{
   unsigned int  addr = inet_addr(ipAddr);
   int i;


	addr = htonl(addr); //add  by paul
	
   if (addr == INADDR_NONE) {
      return 32;
   } else {
      for (i=0; i<32;i++) {
         if ((addr>>i) & 1) {
            break;
         }
     }
     return 32 - i;
   }
}



static int getNums(char *value, char delimit)
{
	char *pos = value;
    int count=1;
    if(!pos)
    	return 0;
	while( (pos = strchr(pos, delimit))){
		pos = pos+1;
		count++;
	}
	return count;
}


/*
inline int getRuleNums(char *rules){
	return getNums(rules, ';');
}

*/
/*
 * ASP function
 */
static int getIPSECASPRuleNumsASP(int eid, webs_t wp, int argc, char_t **argv)
{
	int irules = 0;
	char *en_rules = nvram_bufget(RT2860_NVRAM, "IPSECRules");

	if (en_rules && strlen(en_rules))
		irules += getRuleNums(en_rules);
	websWrite(wp, T("%d"),  irules);
	return 0;
}

static int showIPSECASP(int eid, webs_t wp, int argc, char_t **argv)
{
	int i = 0;
	char rec[196];
	char connName[32];
	char remoteGWAddr[32], *localIPMode[16], *localIP[32], *localMask[32],  *remoteIP[32];
	char service_mod_sel[32];
	char states[8];
	char *pstates = NULL;
	char *rules = nvram_bufget(RT2860_NVRAM, "IPSECRules");
	char *language = nvram_bufget(RT2860_NVRAM, "Language");
	if(!rules)
		return 0;
	if(!strlen(rules))
		return 0;
	
	while(getNthValueSafe(i++, rules, ';', rec, sizeof(rec)) != -1 ){
		// get states 
		if((getNthValueSafe(0, rec, ',', states, sizeof(states)) == -1)){
			continue;
		}


		// get connname
		if((getNthValueSafe(1, rec, ',', connName, sizeof(connName)) == -1)){
			continue;
		}


		// get service mode
		if((getNthValueSafe(2, rec, ',', service_mod_sel, sizeof(service_mod_sel)) == -1)){
			continue;
		}
		if(strstr(service_mod_sel,"client"))
		{	
			// remoteGWAddr
			if((getNthValueSafe(3, rec, ',', remoteGWAddr, sizeof(remoteGWAddr)) == -1)){
				continue;
			}
			/*
			if(!isIpValid(remoteGWAddr)){
				continue;
			}
			*/
		}else{
			strcpy(remoteGWAddr,"");
		} 
		
		// get localIPMode
		if((getNthValueSafe(4, rec, ',', localIPMode, sizeof(localIPMode)) == -1)){
			continue;
		}

		//get localIP
		if((getNthValueSafe(5, rec, ',', localIP, sizeof(localIP)) == -1)){
			continue;
		}
		if(!isIpValid(localIP)){
			continue;
		}

		//get localMask
		if((getNthValueSafe(6, rec, ',', localMask, sizeof(localMask)) == -1)){
			continue;
		}

		//get remoteIP
		if((getNthValueSafe(7, rec, ',', remoteIP, sizeof(remoteIP)) == -1)){
			continue;
		}
		if(!isIpValid(remoteIP)){
			continue;
		}

		websWrite(wp, T("<tr>\n"));
		// output No.

		if(states[1] == '0')
		websWrite(wp, T("<td align=center> %d&nbsp; <input type=\"checkbox\" name=\"actRule%d\"> </td>"), i, i-1 );
		else
		websWrite(wp, T("<td align=center> %d&nbsp; <input type=\"checkbox\" checked=\"checked\" name=\"actRule%d\"> </td>"), i, i-1 );
		// output statues
		//pstates = strchr(states, '|');

		//if (pstates)
			//{
/*
		if(states[1] == '0'){
			if (!strcmp(language,"en"))
				websWrite(wp, T("<td align=center>Disabled</td>"));
			if (!strcmp(language,"zhcn"))
				websWrite(wp, T("<td align=center>关闭</td>"));
			if (!strcmp(language,"zhtw"))
				websWrite(wp, T("<td align=center>關閉</td>"));
			}
		else
			{
			if (!strcmp(language,"en"))
				websWrite(wp, T("<td align=center>Enabled</td>"));
			if (!strcmp(language,"zhcn"))
				websWrite(wp, T("<td align=center>开启</td>"));
			if (!strcmp(language,"zhtw"))
				websWrite(wp, T("<td align=center>開啟</td>"));
			}
*/			
		if(states[1] == '0'){
				websWrite(wp, T("<td id=\"vpn_status_disable\" align=center>Disabled</td>"));
			}
		else
			{
				websWrite(wp, T("<td id=\"vpn_status_enable\" align=center>Enabled</td>"));
			}
		
			//}

		// output name	
		websWrite(wp, T("<td align=center> %s </td>"), connName);
			
		// output service mode	
//		websWrite(wp, T("<td align=center > %s </td>"), service_mod_sel);
		websWrite(wp, T("<td align=center id=\"option_%s\"  > %s </td>"), service_mod_sel,service_mod_sel);


		// output remoteGWAddr 

		websWrite(wp, T("<td align=center> %s </td>"), remoteGWAddr);

		// output localIP 

		websWrite(wp, T("<td align=center> %s </td>"), localIP);

		// output remoteIP 

		websWrite(wp, T("<td align=center> %s </td>"), remoteIP);

		
		websWrite(wp, T("</tr>\n"));
	}
	return 0;



}


/* goform/setipsec_cinfig */
static void setipsec_config(webs_t wp, char_t *path, char_t *query)
{
	char * vpneditsign;
	char* buf_temp[1024];
	char* edit_num_str;
	int edit_num;
	char* run_status;
	int sign_edit=0;
	char* rec;
	char* p;
	int i;


	char rulesbuf[10240] = {0};
	char *IPSECRules;
	
	char *connName;
	char *service_mode_sel;
	char *remoteGWAddr, *localIPMode, *localIP, *localMask, *remoteIPMode, *remoteIP, *remoteMask;
	char *keyExM, *authM, *psk, *certificateName,*perfectFSEn;//key exenge 方法
	/* 高级配置 */
	char *manualEncryptionAlgo, *manualEncryptionKey, *manualAuthAlgo, *manualAuthKey, *spi;

	/*   phase 1 */
	char *ph1Mode, *ph1EncryptionAlgo, *ph1IntegrityAlgo, *ph1DHGroup, *ph1KeyTime;

	/*   phase 2 */
	char *ph2EncryptionAlgo, *ph2IntegrityAlgo, *ph2DHGroup, *ph2KeyTime;
 

	char *nat_traversal;
	char*policies_remote_lan;

	//int ph1KeyTimetmp,ph2KeyTimetmp;

	
 	connName = websGetVar(wp, T("connName"), T(""));
 	service_mode_sel = websGetVar(wp, T("serviceselbox"), T(""));
	
	remoteGWAddr = websGetVar(wp, T("remoteGWAddr"), T(""));
	localIPMode = websGetVar(wp, T("localIPMode"), T(""));
	localIP = websGetVar(wp, T("localIP"), T(""));
	localMask = websGetVar(wp, T("localMask"), T(""));
	remoteIP = websGetVar(wp, T("remoteIP"), T(""));	
	remoteIPMode = websGetVar(wp, T("remoteIPMode"), T(""));
	remoteMask = websGetVar(wp, T("remoteMask"), T(""));
	keyExM = websGetVar(wp, T("keyExM"), T(""));
	authM = websGetVar(wp, T("authM"), T(""));
	psk = websGetVar(wp, T("psk"), T(""));
	certificateName = websGetVar(wp, T("certificateName"), T(""));//zhengshuwenti
	perfectFSEn = websGetVar(wp, T("perfectFSEn"), T(""));
	manualEncryptionAlgo = websGetVar(wp, T("manualEncryptionAlgo"), T(""));
	manualEncryptionKey = websGetVar(wp, T("manualEncryptionKey"), T(""));
	manualAuthAlgo = websGetVar(wp, T("manualAuthAlgo"), T(""));
	manualAuthKey = websGetVar(wp, T("manualAuthKey"), T(""));
	spi = websGetVar(wp, T("spi"), T(""));
	ph1Mode = websGetVar(wp, T("ph1Mode"), T(""));
	ph1EncryptionAlgo = websGetVar(wp, T("ph1EncryptionAlgo"), T(""));
	ph1IntegrityAlgo = websGetVar(wp, T("ph1IntegrityAlgo"), T(""));
	ph1DHGroup = websGetVar(wp, T("ph1DHGroup"), T(""));
	ph1KeyTime = websGetVar(wp, T("ph1KeyTime"), T(""));
	ph2EncryptionAlgo = websGetVar(wp, T("ph2EncryptionAlgo"), T(""));
	ph2IntegrityAlgo = websGetVar(wp, T("ph2IntegrityAlgo"), T(""));
	ph2DHGroup = websGetVar(wp, T("ph2DHGroup"), T(""));
	ph2KeyTime = websGetVar(wp, T("ph2KeyTime"), T(""));

	nat_traversal = websGetVar(wp, T("nat_traversal"), T(""));
	policies_remote_lan= websGetVar(wp, T("policies_remote_lan"), T(""));
 

	IPSECRules = nvram_bufget(RT2860_NVRAM, "IPSECRules");
	sign_edit=0;
	vpneditsign=nvram_bufget(RT2860_NVRAM,"VpnConfigEditSign");		
	if(strstr(vpneditsign,"dis")){  //add rules


						if( IPSECRules && strlen( IPSECRules) )
							snprintf(rulesbuf, sizeof(rulesbuf), "%s;%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", 
										IPSECRules,"|0|",connName,service_mode_sel,remoteGWAddr,localIPMode,localIP,localMask,remoteIP,remoteIPMode,remoteMask,keyExM,
										authM,psk,certificateName,perfectFSEn,manualEncryptionAlgo,manualEncryptionKey,manualAuthAlgo,manualAuthKey,spi,ph1Mode,
										ph1EncryptionAlgo,ph1IntegrityAlgo,ph1DHGroup,ph1KeyTime,
										ph2EncryptionAlgo,ph2IntegrityAlgo,ph2DHGroup,ph2KeyTime,
										nat_traversal,policies_remote_lan);

						else
							snprintf(rulesbuf, sizeof(rulesbuf), "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", 
										"|0|",connName,service_mode_sel,remoteGWAddr,localIPMode,localIP,localMask,remoteIP,remoteIPMode,remoteMask,keyExM,
										authM,psk,certificateName,perfectFSEn,manualEncryptionAlgo,manualEncryptionKey,manualAuthAlgo,manualAuthKey,spi,ph1Mode,
										ph1EncryptionAlgo,ph1IntegrityAlgo,ph1DHGroup,ph1KeyTime,
										ph2EncryptionAlgo,ph2IntegrityAlgo,ph2DHGroup,ph2KeyTime,
										nat_traversal,policies_remote_lan);

		}else{ //edit rules
						edit_num_str=strchr(vpneditsign,'_');	
				if(edit_num_str){
						edit_num_str++;
						edit_num=atoi(edit_num_str);
						if((edit_num<IPSEC_MAX_RULES)&&(edit_num>=0)){
									run_status=nvram_bufget(RT2860_NVRAM,"VpnConfigEdit_status");
									memset(buf_temp,0,sizeof(buf_temp));
									snprintf(buf_temp, sizeof(buf_temp), "%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s", 
										run_status,connName,service_mode_sel,remoteGWAddr,localIPMode,localIP,localMask,remoteIP,remoteIPMode,remoteMask,keyExM,
										authM,psk,certificateName,perfectFSEn,manualEncryptionAlgo,manualEncryptionKey,manualAuthAlgo,manualAuthKey,spi,ph1Mode,
										ph1EncryptionAlgo,ph1IntegrityAlgo,ph1DHGroup,ph1KeyTime,
										ph2EncryptionAlgo,ph2IntegrityAlgo,ph2DHGroup,ph2KeyTime,
										nat_traversal,policies_remote_lan);
									
										p=IPSECRules;	
										if(edit_num!=0){
											for(i=0;i<edit_num;i++){
												 p=strchr(p,';');
												p++;
											}
											rec=p;
										}else{
											rec=IPSECRules;
										}
										
										strncpy(rulesbuf,IPSECRules,rec-IPSECRules);
										strcat(rulesbuf,buf_temp);
										p=strchr(p,';');
										if(p!=NULL)
										{
											p++;
											rec=p;
											strcat(rulesbuf,";");
											strcat(rulesbuf,rec);								
										}
										if(*(run_status+1)=='1')	sign_edit=1;
						}
				}
		nvram_bufset(RT2860_NVRAM,"VpnConfigEditSign","dis");
	}
	
	//printf("rulesbuf:%s\n", rulesbuf);
	nvram_set(RT2860_NVRAM, "IPSECRules", rulesbuf);
	nvram_commit(RT2860_NVRAM);

	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "html/ipsecVpn.asp");


	if(sign_edit==1){
			ipsec_run();
	}


	return;


}



//-1 error
//0  pass
// 1  have dev and id error in client mode
int check_dev(char* ipsec_rules)
{
	FILE*fp;
	char *p,*p1;
	char devid[30];
	int len,i;

	if(ipsec_rules==NULL) return -1;
	
	//return if no dev
	if((p=strstr(ipsec_rules,"DEV"))==NULL){
		return 0;
	}

	//get id
	memset(devid,0,sizeof(devid));
	if((fp=popen("get_dev_id","r"))==NULL){
		return -1;
	}
	if(fgets(devid,sizeof(devid),fp)==NULL){
		return -1;
	}
	if(fp) pclose(fp);
	
	if(strstr(devid,"error")){
		return -1;
	}
	
	p=ipsec_rules;
	while(1){	
		if( (p1=strstr(p,"DEV"))!=NULL){    //DEV+ID   ID error
		
			if(strncmp(p1+3,devid,strlen(devid))) {
				
				p1=strchr(p1,','); 
				p1++;
				if(! strncmp(p1,"client",6)){
					return 1;   //id error
				}
			}
			p=p1+1;
		}else{
			return 0;//pass
		}
	}
}




int change_host_to_ip(char* host,char* ip)
{
	FILE*fp;
	char buf[30];
	char cmd[100];

	if((host==NULL)||(ip==NULL)) return -1;
	memset(buf,0,sizeof(buf));
	memset(cmd,0,sizeof(cmd));

	strcpy(cmd,"host_to_ip ");
	strcat(cmd,host);

	if((fp=popen(cmd,"r"))==NULL) {
		return -1;
	}
	fgets(buf,sizeof(buf),fp);
	if(fp) pclose(fp);

	if(strstr(buf,"error")){
		return -1;
	}
	strcpy(ip,buf);
	return 0;	
}




void ipsec_run(void){

/*


	int have_server_remote_flag=0;	
	static int flag = 0 ;
	FILE *fp;
   	char wanIP[32];
   	int local_prefixlen;
   	int remote_prefixlen;
  	char cmd[256];
	int i = 0;
	char rec[800];
	char states[8];
	char connName[32];
	char service_mod_sel[32];
	char remoteGWAddr[32], localIPMode[8], localIP[32], localMask[32],remoteIPMode[8],remoteIP[32],remoteMask[32];
	char keyExM[8], authM[16], psk[64], certificateName[32],perfectFSEn[8];//key exenge 方法


	char manualEncryptionAlgo[16], manualEncryptionKey[80], manualAuthAlgo[16], manualAuthKey[60], spi[8];

	char ph1Mode[16], ph1EncryptionAlgo[8], ph1IntegrityAlgo[8], ph1DHGroup[10], ph1KeyTime[16];
	

	char ph2EncryptionAlgo[8], ph2IntegrityAlgo[12], ph2DHGroup[12], ph2KeyTime[16];

	char *pstates = NULL;
	char *rules = nvram_bufget(RT2860_NVRAM, "IPSECRules");


	char remoteGWAddr_tmp[32];




	//add for VPN use Device check
	int checkdev_ret;
	checkdev_ret=check_dev(rules);
	if(checkdev_ret==1){
		printf("VPN: invalid vpn name, DEV ID error when use DEV+ID format!!!\n");
		return;
	}else if(checkdev_ret==-1){
		printf("VPN: Vpn check handle error!!!\n");
		return;
	}





	//判断WAN 口有没有连接
	if (-1 == getIfIp(getWanIfNamePPP(), wanIP))
		{
		 //do nothing
			printf("Can't get Wan IP\n");
		 	return;
		}




	doSystem("rm -rf /var/racoon.conf");
	doSystem("rm -rf /var/setkey.conf");
	doSystem("rm -rf /var/psk.txt");	










	//printf("wanIP==%s\n",wanIP);
	//printf("getWanIfNamePPP==%s\n",getWanIfNamePPP());
  
	//创建配置文件开始部分

	fp = fopen("/var/setkey.conf", "a+");
			
		 if (fp == NULL) {      
				printf("app/ipsec: unable to open setkey file\n");      
				return ;   
		}  
		fprintf(fp, "flush;\n");   
		fprintf(fp, "spdflush;\n");
		fclose(fp);//close file


   	fp = fopen("/var/racoon.conf", "a+");
  	 	if (fp == NULL) {
     	 printf("app/ipsec: unable to open racoon file\n");
     	 return ;
  		 }
   		fprintf(fp, "path pre_shared_key \"/var/psk.txt\";\n");
   		fprintf(fp, "path certificate \"%s\";\n\n", CERT_PATH);
		fclose(fp);//close file


		
	if(!rules)
		return ;
	if(!strlen(rules))
		return ;


	
	while(getNthValueSafe(i++, rules, ';', rec, sizeof(rec)) != -1 ){
		// get states 
		if((getNthValueSafe(0, rec, ',', states, sizeof(states)) == -1)){
			continue;
		}


		// get connname
		if((getNthValueSafe(1, rec, ',', connName, sizeof(connName)) == -1)){
			continue;
		}





		// get service mode sel
		if((getNthValueSafe(2, rec, ',', service_mod_sel, sizeof(service_mod_sel)) == -1)){
			continue;
		}


		if(strstr(service_mod_sel,"client")){
		
			// remoteGWAddr
			if((getNthValueSafe(3, rec, ',', remoteGWAddr, sizeof(remoteGWAddr)) == -1)){
				continue;
			}

			memset(remoteGWAddr_tmp,0,sizeof(remoteGWAddr_tmp));
			if(change_host_to_ip(remoteGWAddr,remoteGWAddr_tmp)!=-1){ //OK
				memset(remoteGWAddr,0,sizeof(remoteGWAddr));	
				strcpy(remoteGWAddr,remoteGWAddr_tmp);
			}
			
			if(!isIpValid(remoteGWAddr)){
				continue;
			}
		}

		// get localIPMode
		if((getNthValueSafe(4, rec, ',', localIPMode, sizeof(localIPMode)) == -1)){
			continue;
		}

		//get localIP
		if((getNthValueSafe(5, rec, ',', localIP, sizeof(localIP)) == -1)){
			continue;
		}
		if(!isIpValid(localIP)){
			continue;
		}

		//get localMask
		if((getNthValueSafe(6, rec, ',', localMask, sizeof(localMask)) == -1)){
			continue;
		}

		//get remoteIP
		if((getNthValueSafe(7, rec, ',', remoteIP, sizeof(remoteIP)) == -1)){
			continue;
		}
		if(!isIpValid(remoteIP)){
			continue;
		}

		//get remoteIPMode
		if((getNthValueSafe(8, rec, ',', remoteIPMode, sizeof(remoteIPMode)) == -1)){
			continue;
		}

		//get remoteMask
		if((getNthValueSafe(9, rec, ',', remoteMask, sizeof(remoteMask)) == -1)){
			continue;
		}


		//get keyExM
		if((getNthValueSafe(10, rec, ',', keyExM, sizeof(keyExM)) == -1)){
			continue;
		}


		//get authM
		if((getNthValueSafe(11, rec, ',', authM, sizeof(authM)) == -1)){
			continue;
		}		
		
		//get psk
		if((getNthValueSafe(12, rec, ',', psk, sizeof(psk)) == -1)){
			continue;
		}

		//get certificateName
		if((getNthValueSafe(13, rec, ',', certificateName, sizeof(certificateName)) == -1)){
			continue;
		}

		//get perfectFSEn
		if((getNthValueSafe(14, rec, ',', perfectFSEn, sizeof(perfectFSEn)) == -1)){
			continue;
		}

		//get manualEncryptionAlgo
		if((getNthValueSafe(15, rec, ',', manualEncryptionAlgo, sizeof(manualEncryptionAlgo)) == -1)){
			continue;
		}

								
		//get manualEncryptionKey
		if((getNthValueSafe(16, rec, ',', manualEncryptionKey, sizeof(manualEncryptionKey)) == -1)){
			continue;
		}
		//get manualAuthAlgo
		if((getNthValueSafe(17, rec, ',', manualAuthAlgo, sizeof(manualAuthAlgo)) == -1)){
			continue;
		}

		//get manualAuthKey
		if((getNthValueSafe(18, rec, ',', manualAuthKey, sizeof(manualAuthKey)) == -1)){
			continue;
		}

		//get spi
		if((getNthValueSafe(19, rec, ',', spi, sizeof(spi)) == -1)){
			continue;
		}

		//get ph1Mode
		if((getNthValueSafe(20, rec, ',', ph1Mode, sizeof(ph1Mode)) == -1)){
			continue;
		}

		//get ph1EncryptionAlgo
		if((getNthValueSafe(21, rec, ',', ph1EncryptionAlgo, sizeof(ph1EncryptionAlgo)) == -1)){
			continue;
		}

		//get ph1IntegrityAlgo
		if((getNthValueSafe(22, rec, ',', ph1IntegrityAlgo, sizeof(ph1IntegrityAlgo)) == -1)){
			continue;
		}

		//get ph1DHGroup
		if((getNthValueSafe(23, rec, ',', ph1DHGroup, sizeof(ph1DHGroup)) == -1)){
			continue;
		}

		//get ph1KeyTime
		if((getNthValueSafe(24, rec, ',', ph1KeyTime, sizeof(ph1KeyTime)) == -1)){
			continue;
		}

		//get ph2EncryptionAlgo
		if((getNthValueSafe(25, rec, ',', ph2EncryptionAlgo, sizeof(ph2EncryptionAlgo)) == -1)){
			continue;
		}

		//get ph2IntegrityAlgo
		if((getNthValueSafe(26, rec, ',', ph2IntegrityAlgo, sizeof(ph2IntegrityAlgo)) == -1)){
			continue;
		}

		//get ph2DHGroup
		if((getNthValueSafe(27, rec, ',', ph2DHGroup, sizeof(ph2DHGroup)) == -1)){
			continue;
		}

		//get ph2KeyTime
		if((getNthValueSafe(28, rec, ',', ph2KeyTime, sizeof(ph2KeyTime)) == -1)){
			continue;
		}

		
		if(states[1] == '0')   //关闭状态不生成配置文件
		{
			continue;
		}
		else{


			if (!strcmp(localIPMode, "subnet"))

				 local_prefixlen = rutIPSec_calPrefixLen(localMask);

			 	else
					local_prefixlen = 32;


			 if (!strcmp(remoteIPMode, "subnet"))

				 remote_prefixlen = rutIPSec_calPrefixLen(remoteMask);
			 	else
					remote_prefixlen = 32;
				


			//setkey.conf
			 fp = fopen("/var/setkey.conf", "a+");
			
			 if (fp == NULL) {      
				printf("app/ipsec: unable to open file\n");      
				return ;   
			 }  

		     if (!strcmp(keyExM, "manual")) {


		         fprintf(fp, "add %s %s esp 0x%s -m tunnel -E %s 0x%s -A %s 0x%s;\n",
                 		wanIP, remoteGWAddr, spi,manualEncryptionAlgo,manualEncryptionKey,manualAuthAlgo, manualAuthKey);
        		 fprintf(fp, "add %s %s esp 0x%s -m tunnel -E %s 0x%s -A %s 0x%s;\n",
                 		remoteGWAddr, wanIP, spi,manualEncryptionAlgo, manualEncryptionKey,manualAuthAlgo,manualAuthKey);
		
			 

		
		      	fprintf(fp, "spdadd %s/%d %s/%d any -P out ipsec esp/tunnel/%s-%s/require;\n",
             			localIP, local_prefixlen, remoteIP, remote_prefixlen, wanIP,remoteGWAddr);
     		  	fprintf(fp, "spdadd %s/%d %s/%d any -P in ipsec esp/tunnel/%s-%s/require;\n",
              			remoteIP, remote_prefixlen, localIP, local_prefixlen,remoteGWAddr, wanIP);
			}
			else	 if(!strcmp(keyExM, "auto")){
	 			    if (!strcmp(service_mod_sel, "client")) {
					   	fprintf(fp, "spdadd %s/%d %s/%d any -P out ipsec esp/tunnel/%s-%s/require;\n",
             							localIP, local_prefixlen, remoteIP, remote_prefixlen, wanIP,remoteGWAddr);
     		  				fprintf(fp, "spdadd %s/%d %s/%d any -P in ipsec esp/tunnel/%s-%s/require;\n",
              						remoteIP, remote_prefixlen, localIP, local_prefixlen,remoteGWAddr, wanIP);
						
	 			    	}

				
				}
			 
			 
	
			 

				fclose(fp);//close file

				//disable masquegrading for ipsec tunnel
     			sprintf(cmd, "iptables -t nat -I POSTROUTING 1 -s %s/%d -o %s -d %s/%d -j ACCEPT",
                   		localIP, local_prefixlen, getWanIfNamePPP(), remoteIP, remote_prefixlen);
      					doSystem(cmd);

      			printf ("ipsec:spdadd Done\n");


			 //racoon

			
      	if (!strcmp(keyExM, "auto") ){


		fp = fopen("/var/racoon.conf", "a+");
  	 		if (fp == NULL) {
     		 printf("app/ipsec: unable to open racoon file\n");
     		 return ;
  			 }








	if (!strcmp(service_mod_sel, "client")) {
	      	   fprintf(fp, "remote %s {\n", remoteGWAddr);
       	  fprintf(fp, "exchange_mode %s;\n", ph1Mode);
		  if (!strcmp(authM, "pre_shared_key")) {
		  fprintf(fp, "my_identifier fqdn \"%s\";\n", connName);
		  	}


			/////////////////////////////////////////////////////
         fprintf(fp, "lifetime time %s sec;\n", ph1KeyTime);
         fprintf(fp, "proposal_check obey;\n");

         if (!strcmp(authM, "pre_shared_key")) {
            fprintf(fp, "proposal {\n");
            fprintf(fp, "encryption_algorithm %s;\n", ph1EncryptionAlgo);
            fprintf(fp, "hash_algorithm %s;\n", ph1IntegrityAlgo);
            fprintf(fp, "authentication_method %s;\n", "pre_shared_key");
            fprintf(fp, "dh_group %s;\n", ph1DHGroup);
            fprintf(fp, "}\n");
            fprintf(fp, "}\n\n");
         }

         else if (strcmp(authM, "certificate")) {
            fprintf(fp, "verify_cert on;\n");
            fprintf(fp, "my_identifier asn1dn;\n");
            fprintf(fp, "peers_identifier asn1dn;\n");
            fprintf(fp, "certificate_type x509 \"%s.cert\" \"%s.priv\";\n",
                    certificateName, certificateName);
            fprintf(fp, "proposal {\n");
            fprintf(fp, "encryption_algorithm %s;\n", ph1EncryptionAlgo);
            fprintf(fp, "hash_algorithm %s;\n", ph1IntegrityAlgo);
            fprintf(fp, "authentication_method %s;\n", "rsasig");
            fprintf(fp, "dh_group %s;\n", ph1DHGroup);
            fprintf(fp, "}\n");
            fprintf(fp, "}\n\n");
         }
		 else { 
            // unknown type  do nothing
		}

			   
	}else if((!strcmp(service_mod_sel, "service"))&&(have_server_remote_flag==0)){
				have_server_remote_flag=1;
			fprintf(fp, "remote anonymous {\n");
       	   	 fprintf(fp, "exchange_mode aggressive,main;\n");
			  if (!strcmp(authM, "pre_shared_key")) {
//			fprintf(fp, "peers_identifier fqdn \"%s\";\n", connName);
			  	}
			fprintf(fp, "generate_policy on;\n");
			fprintf(fp, "passive on;\n");

			/////////////////////////////////////////////////////
         fprintf(fp, "lifetime time %s sec;\n", ph1KeyTime);
         fprintf(fp, "proposal_check obey;\n");

         if (!strcmp(authM, "pre_shared_key")) {
            fprintf(fp, "proposal {\n");
            fprintf(fp, "encryption_algorithm %s;\n", ph1EncryptionAlgo);
            fprintf(fp, "hash_algorithm %s;\n", ph1IntegrityAlgo);
            fprintf(fp, "authentication_method %s;\n", "pre_shared_key");
            fprintf(fp, "dh_group %s;\n", ph1DHGroup);
            fprintf(fp, "}\n");
            fprintf(fp, "}\n\n");
         }

         else if (strcmp(authM, "certificate")) {
            fprintf(fp, "verify_cert on;\n");
            fprintf(fp, "my_identifier asn1dn;\n");
            fprintf(fp, "peers_identifier asn1dn;\n");
            fprintf(fp, "certificate_type x509 \"%s.cert\" \"%s.priv\";\n",
                    certificateName, certificateName);
            fprintf(fp, "proposal {\n");
            fprintf(fp, "encryption_algorithm %s;\n", ph1EncryptionAlgo);
            fprintf(fp, "hash_algorithm %s;\n", ph1IntegrityAlgo);
            fprintf(fp, "authentication_method %s;\n", "rsasig");
            fprintf(fp, "dh_group %s;\n", ph1DHGroup);
            fprintf(fp, "}\n");
            fprintf(fp, "}\n\n");
         }
		 else { 
            // unknown type  do nothing
         }
			
	}//end server
	

	

         fprintf(fp, "sainfo address %s/%d any address %s/%d any {\n",
                 localIP, local_prefixlen, remoteIP, remote_prefixlen);
         if (!strcmp(perfectFSEn, "enable")) {
            fprintf(fp, "pfs_group %s;\n", ph2DHGroup);
         }
         fprintf(fp, "lifetime time %s sec;\n", ph2KeyTime);
         fprintf(fp, "encryption_algorithm %s;\n", ph2EncryptionAlgo);
         fprintf(fp, "authentication_algorithm %s;\n", ph2IntegrityAlgo);
         fprintf(fp, "compression_algorithm deflate;\n");
         fprintf(fp, "}\n\n");

		 fclose(fp);//close   racoon

		}


		// psk  psk.txt
		
   		fp = fopen("/var/psk.txt", "a+");
   		if (fp == NULL) {
      		printf("app/ipsec: unable to open file\n");
     			 return ;}

      	if ((!strcmp(keyExM, "auto")) && (!strcmp(authM, "pre_shared_key"))) {
			if (strstr(service_mod_sel, "client")) {
         				fprintf(fp, "%s %s\n", remoteGWAddr, psk);
						
			}else if(strstr(service_mod_sel, "service")){
					fprintf(fp, "%s %s\n", connName, psk);
					
				}
     	 	}
     
   		fclose(fp); //close psk.txt
			

		}
	
	}


	doSystem("chmod 600 /var/psk.txt");
	doSystem("chmod 640 /var/setkey.conf");
	doSystem("chmod 640 /var/racoon.conf");




	
	doSystem("killall start_vpn");
	sleep(1);
	doSystem("killall racoon");
	sleep(1);
	//doSystem("start_vpn&");
	doSystem("setkey -f var/setkey.conf");
	doSystem("racoon -f var/racoon.conf");
*/


	doSystem("killall start_vpn");
	sleep(1);
	doSystem("killall racoon");
	sleep(1);
	doSystem("start_vpn&");


	return;

}

static void IpsecAction(webs_t wp, char_t *path, char_t *query)
{
	int i, j, rule_count;
	char *actionType;
	int *actArray = NULL;
	char_t name_buf[16];
	char flgEnable;
	char *pRule = NULL;
	char_t *value;
	char *rules = nvram_bufget(RT2860_NVRAM, "IPSECRules");
	char *p_ch = NULL;
	int edit_sign=0;

	if(!rules || !strlen(rules) )
		goto end;
	rule_count = getRuleNums(rules);
	if(!rule_count)
		goto end;

	actionType = websGetVar(wp, T("actionType"), NULL);
	if (!actionType)
		goto end;
	//printf("action:%s\n", actionType);
	/* unknown action */
	if ( (0 != strcmp(actionType, "en"))	//enable
		&& (0 != strcmp(actionType, "dis"))	//disable
		&& (0 != strcmp(actionType, "del")) 	//delete
		&& (0 != strcmp(actionType, "edt")) )	//delete
		goto end;

	actArray = (int *)malloc(rule_count * sizeof(int));
	if (!actArray) {
		perror("malloc");
		goto end;
	}

	for(i=0, j=0; i< rule_count; i++){
		snprintf(name_buf, 16, "actRule%d", i);
		value = websGetVar(wp, T(name_buf), NULL);
		if(value){
			actArray[j++] = i;
		}
	}
	if(0==strcmp(actionType,"edt")){
		if((j!=1)){
			edit_sign=0;
			nvram_bufset(RT2860_NVRAM, "VpnConfigEditSign", "dis");
			goto end;
		}	
			
		sprintf(name_buf,"edt_%d",actArray[0]);	
		nvram_bufset(RT2860_NVRAM, "VpnConfigEditSign", name_buf);
		edit_sign=1;
		goto end;	
	}else if ( (0 == strcmp(actionType, "en")) || (0 == strcmp(actionType, "dis")) ){
		flgEnable = (actionType[0] == 'e') ? '1' : '0';
		for (i = 0; i < j; i++){
			pRule = getNthValueOffset(actArray[i], rules);
			//printf("rule1:%s\n", pRule);
			if (pRule)
				p_ch = strchr(pRule, '|');
			if (p_ch)
				//*(++pRule) = flgEnable;
				*(p_ch+1) = flgEnable;
		
			//printf("rule2:%s\n", pRule);
		}
	}else if (0 == strcmp(actionType, "del") ){
		deleteNthValueMulti(actArray, rule_count, rules, ';');
		
	}else{
	
		goto end;
	}

	nvram_set(RT2860_NVRAM, "IPSECRules", rules);
	nvram_commit(RT2860_NVRAM);
	
	if (actArray) 

		free(actArray); //release memory
	
	websRedirect(wp, "html/ipsecVpn.asp");
	
	/* make config  file */
	ipsec_run();

	return;

end:
	if (actArray) 
		free(actArray); //release memory
	if(edit_sign==1)
	{
		vpn_edit_load_para();
		websRedirect(wp, "internet/ipsconfig_edit.asp");
	}
	else	
		websRedirect(wp, "html/ipsecVpn.asp");
	return;
}

static int vpn_edit_load_para(void)
{
	char* vpneditsign;
	char* edit_num_str;
	int edit_num=0;
	char *rules = nvram_bufget(RT2860_NVRAM, "IPSECRules");
	char tmp[200];

	char rec[1024];
	char states[8];
	char connName[32];
	char service_mod_sel[32];
	char remoteGWAddr[32], localIPMode[8], localIP[32], localMask[32],remoteIPMode[8],remoteIP[32],remoteMask[32];
	char keyExM[8], authM[16], psk[64], certificateName[32],perfectFSEn[8];//key exenge 方法

/* 高级配置 */
	char manualEncryptionAlgo[16], manualEncryptionKey[80], manualAuthAlgo[16], manualAuthKey[60], spi[8];
	/*   phase 1 */
	char ph1Mode[16], ph1EncryptionAlgo[8], ph1IntegrityAlgo[8], ph1DHGroup[10], ph1KeyTime[16];
	
	/*   phase 2 */
	char ph2EncryptionAlgo[8], ph2IntegrityAlgo[12], ph2DHGroup[12], ph2KeyTime[16];
	char nat_traversal[5];
	char policies_remote_lan[200];
	

	char *pstates = NULL;



					if(!rules)
						return 0;
					if(!strlen(rules))
						return 0;




	vpneditsign=nvram_bufget(RT2860_NVRAM,"VpnConfigEditSign");		
	if(!strstr(vpneditsign,"dis")){
		
		edit_num_str=strchr(vpneditsign,'_');	
		if(edit_num_str){
			edit_num_str++;
			edit_num=atoi(edit_num_str);
			if(edit_num<IPSEC_MAX_RULES){
				



					
					getNthValueSafe(edit_num, rules, ';', rec, sizeof(rec));
					if(rec==NULL) return -1;




						// get states 
						if((getNthValueSafe(0, rec, ',', states, sizeof(states)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_status",states);		


						// get connname
						if((getNthValueSafe(1, rec, ',', connName, sizeof(connName)) == -1)){
							return -1;
						}
					//	nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_connName",connName);		
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_connName",connName);		


						// get service mode sel
						if((getNthValueSafe(2, rec, ',', service_mod_sel, sizeof(service_mod_sel)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_service_mod_sel",service_mod_sel);		


						if(strstr(service_mod_sel,"client")){
						
							// remoteGWAddr
							if((getNthValueSafe(3, rec, ',', remoteGWAddr, sizeof(remoteGWAddr)) == -1)){
								return -1;
							}
							/*
							if(!isIpValid(remoteGWAddr)){
								return -1;
							}
							*/
							nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_remoteGWAddr",remoteGWAddr);		
							
						}else{
							nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_remoteGWAddr","");		
						}
						// get localIPMode
						if((getNthValueSafe(4, rec, ',', localIPMode, sizeof(localIPMode)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_localIPMode",localIPMode);		

						//get localIP
						if((getNthValueSafe(5, rec, ',', localIP, sizeof(localIP)) == -1)){
							return -1;
						}
						if(!isIpValid(localIP)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_localIP",localIP);		

						//get localMask
						if((getNthValueSafe(6, rec, ',', localMask, sizeof(localMask)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_localMask",localMask);		
						

						//get remoteIP
						if((getNthValueSafe(7, rec, ',', remoteIP, sizeof(remoteIP)) == -1)){
							return -1;
						}
						if(!isIpValid(remoteIP)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_remoteIP",remoteIP);		

						//get remoteIPMode
						if((getNthValueSafe(8, rec, ',', remoteIPMode, sizeof(remoteIPMode)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_remoteIPMode",remoteIPMode);		

						//get remoteMask
						if((getNthValueSafe(9, rec, ',', remoteMask, sizeof(remoteMask)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_remoteMask",remoteMask);		

						//get keyExM
						if((getNthValueSafe(10, rec, ',', keyExM, sizeof(keyExM)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_keyExM",keyExM);		


						//get authM
						if((getNthValueSafe(11, rec, ',', authM, sizeof(authM)) == -1)){
							return -1;
						}		
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_authM",authM);		
						
						//get psk
						if((getNthValueSafe(12, rec, ',', psk, sizeof(psk)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_psk",psk);		

						//get certificateName
						if((getNthValueSafe(13, rec, ',', certificateName, sizeof(certificateName)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_certificateName",certificateName);		

						//get perfectFSEn
						if((getNthValueSafe(14, rec, ',', perfectFSEn, sizeof(perfectFSEn)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_perfectFSEn",perfectFSEn);		

						//get manualEncryptionAlgo
						if((getNthValueSafe(15, rec, ',', manualEncryptionAlgo, sizeof(manualEncryptionAlgo)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_manualEncryptionAlgo",manualEncryptionAlgo);		

												
						//get manualEncryptionKey
						if((getNthValueSafe(16, rec, ',', manualEncryptionKey, sizeof(manualEncryptionKey)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_manualEncryptionKey",manualEncryptionKey);		
						//get manualAuthAlgo
						if((getNthValueSafe(17, rec, ',', manualAuthAlgo, sizeof(manualAuthAlgo)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_manualAuthAlgo",manualAuthAlgo);		

						//get manualAuthKey
						if((getNthValueSafe(18, rec, ',', manualAuthKey, sizeof(manualAuthKey)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_manualAuthKey",manualAuthKey);		

						//get spi
						if((getNthValueSafe(19, rec, ',', spi, sizeof(spi)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_spi",spi);		

						//get ph1Mode
						if((getNthValueSafe(20, rec, ',', ph1Mode, sizeof(ph1Mode)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph1Mode",ph1Mode);		

						//get ph1EncryptionAlgo
						if((getNthValueSafe(21, rec, ',', ph1EncryptionAlgo, sizeof(ph1EncryptionAlgo)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph1EncryptionAlgo",ph1EncryptionAlgo);		

						//get ph1IntegrityAlgo
						if((getNthValueSafe(22, rec, ',', ph1IntegrityAlgo, sizeof(ph1IntegrityAlgo)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph1IntegrityAlgo",ph1IntegrityAlgo);		

						//get ph1DHGroup
						if((getNthValueSafe(23, rec, ',', ph1DHGroup, sizeof(ph1DHGroup)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph1DHGroup",ph1DHGroup);		

						//get ph1KeyTime
						if((getNthValueSafe(24, rec, ',', ph1KeyTime, sizeof(ph1KeyTime)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph1KeyTime",ph1KeyTime);		

						//get ph2EncryptionAlgo
						if((getNthValueSafe(25, rec, ',', ph2EncryptionAlgo, sizeof(ph2EncryptionAlgo)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph2EncryptionAlgo",ph2EncryptionAlgo);		

						//get ph2IntegrityAlgo
						if((getNthValueSafe(26, rec, ',', ph2IntegrityAlgo, sizeof(ph2IntegrityAlgo)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph2IntegrityAlgo",ph2IntegrityAlgo);		

						//get ph2DHGroup
						if((getNthValueSafe(27, rec, ',', ph2DHGroup, sizeof(ph2DHGroup)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph2DHGroup",ph2DHGroup);		

						//get ph2KeyTime
						if((getNthValueSafe(28, rec, ',', ph2KeyTime, sizeof(ph2KeyTime)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"VpnConfigEdit_ph2KeyTime",ph2KeyTime);		
						//get nat_traversal
						if((getNthValueSafe(29, rec, ',', nat_traversal, sizeof(nat_traversal)) == -1)){
							return -1;
						}
						nvram_bufset(RT2860_NVRAM,"nat_traversal",nat_traversal);	
                                                if((getNthValueSafe(30, rec, ',', policies_remote_lan, sizeof(policies_remote_lan)) == -1)){
                                                        return -1;
                                                }
                                                nvram_bufset(RT2860_NVRAM,"policies_remote_lan",policies_remote_lan);
			}
		}				
	}
	//nvram_bufset(RT2860_NVRAM,"VpnConfigEditSign","dis");		
	return 0;
}

void  clean_VpnConfigEditSign(void)
{
	nvram_bufset(RT2860_NVRAM,"VpnConfigEditSign","dis");		
}
/* call in goahead.c initWebs() */
void formDefineVPN(void)
{


	websFormDefine(T("setipsec_config"), setipsec_config);
	websAspDefine(T("showIPSECASP"), showIPSECASP);
	websAspDefine(T("getIPSECASPRuleNumsASP"), getIPSECASPRuleNumsASP);
	websFormDefine(T("IpsecAction"), IpsecAction);
	websAspDefine(T("clean_VpnConfigEditSign"),clean_VpnConfigEditSign);
	
}


void ipsecvpn_init(void)
{
	doSystem("echo \"ipsec vpn init (iptables)\n \"  ");
	
	doSystem("iptables -I INPUT 2 -p udp --dport 500 -i %s -j ACCEPT",getWanIfNamePPP());
	doSystem("iptables -I INPUT 3 -p esp -i %s -j ACCEPT",getWanIfNamePPP());
	doSystem("iptables -t mangle -p esp -A PREROUTING -i %s -j MARK --set-mark 0x10000000",getWanIfNamePPP());
	doSystem("iptables -I INPUT 4 -p ! esp -i %s -m mark --mark 0x10000000/0x10000000 -j ACCEPT",getWanIfNamePPP());
	doSystem("iptables -I FORWARD 1 -p ! esp -i %s  -m mark --mark 0x10000000/0x10000000 -j ACCEPT",getWanIfNamePPP());

	ipsec_run();
}
