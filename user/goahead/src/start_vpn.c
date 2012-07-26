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
#include<syslog.h>
#include	"internet.h"
#include	"nvram.h"
#include	"webs.h"
#include	"utils.h"
#include 	"firewall.h"

#include	"linux/autoconf.h"  //kernel config
#include	"config/autoconf.h" //user config
#include	"user/busybox/include/autoconf.h" //busybox config

#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <ucontext.h>

/* According to POSIX.1-2001 */
#include <sys/select.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>


#define IPSEC_MAX_RULES 20	
#define CERT_PATH "/var/cert"
/*
 * description: return WAN interface name
 *              0 = bridge, 1 = gateway, 2 = wirelss isp
 */
/*
 * arguments: ifname  - interface name
 *            if_addr - a 16-byte buffer to store ip address
 * description: fetch ip address, netmask associated to given interface name
 */


volatile int sign_link_up=0;
int sign_havelink=0;






static int isIpValid(char *str)
{
	struct in_addr addr;	// for examination
	if( (! strcmp(T("any"), str)) || (! strcmp(T("any/0"), str)))
		return 1;

	if(! (inet_aton(str, &addr))){
		syslog(LOG_INFO, "isIpValid(): %s is not a valid IP address.\n", str);
		return 0;
	}
	return 1;
}





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
#else 
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

int getNthValueSafe(int index, char *value, char delimit, char *result, int len)
{
    int i=0, result_len=0;
    char *begin, *end;

    if(!value || !result || !len)
        return -1;

    begin = value;
    end = strchr(begin, delimit);

    while(i<index && end){
        begin = end+1;
        end = strchr(begin, delimit);
        i++;
    }

    //no delimit
    if(!end){
        if(i == index){
            end = begin + strlen(begin);
            result_len = (len-1) < (end-begin) ? (len-1) : (end-begin);
        }else
            return -1;
    }else
        result_len = (len-1) < (end-begin)? (len-1) : (end-begin);

    memcpy(result, begin, result_len );
    *(result+ result_len ) = '\0';

    return 0;
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


char* PPP_Name;
	int have_server_remote_flag=0;
	static int flag = 0 ;
	FILE *fp;
   	char wanIP[32];
   	int local_prefixlen;
   	int remote_prefixlen;
  	char cmd[256];
	int i = 0;
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

	char *pstates = NULL;
//	char *rules;
	char *rules = nvram_bufget(RT2860_NVRAM, "IPSECRules");

	char remoteGWAddr_tmp[32];
	char nat_traversal[5];

	char policies_remote_lan[200];
        int lan_count;
        char lan_mask_one[20];
	 
 
	int checkdev_ret;

	char *lan_ip=nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
 	char* lan_mask=nvram_bufget(RT2860_NVRAM, "lan_netmask");
	//char *lan_ip;
 	//char* lan_mask;
	int lan_prefix;


	sign_havelink=0;//add by zsf 2010-10-30
	if(strstr(rules,"|1|")==NULL){
		sign_havelink=1;//add by zsf 2010-10-30
		
	}

	//add for VPN use Device check
	checkdev_ret=check_dev(rules);
	if(checkdev_ret==1){
		printf("VPN: invalid vpn name, DEV ID error when use DEV+ID format!!!\n");
		return;
	}else if(checkdev_ret==-1){
		printf("VPN: Vpn check handle error!!!\n");
		return;
	}

	//判断WAN 口有没有连接
	PPP_Name=getWanIfNamePPP();
	if (-1 == getIfIp(PPP_Name, wanIP))
		{
		 //do nothing
	//		printf("Can't get Wan IP\n");
		 	return;
		}
	//printf("wanIP==%s\n",wanIP);
	//printf("getWanIfNamePPP==%s\n",getWanIfNamePPP());
  
	sign_havelink=1;//add by zsf 2010-10-30



    /*清空配置文件，重新生成     */
	system("rm -rf /var/racoon.conf");
	system("rm -rf /var/setkey.conf");
	system("rm -rf /var/psk.txt");	
	





//	system("> /var/psk.txt");	//create psk.txt file

	//创建配置文件开始部分

	fp = fopen("/var/setkey.conf", "a+");
			
		 if (fp == NULL) {      
				printf("app/ipsec: unable to open setkey file\n");      
				return ;   
		}  
		fprintf(fp, "flush;\n");   
		fprintf(fp, "spdflush;\n");

		lan_prefix=rutIPSec_calPrefixLen(lan_mask);
		 fprintf(fp, "spdadd %s/%d %s/%d any -P out none;\n", lan_ip, lan_prefix, lan_ip, lan_prefix);
		 fprintf(fp, "spdadd %s/%d %s/%d any -P in none;\n", lan_ip, lan_prefix, lan_ip, lan_prefix);	     		  			 

		
		fclose(fp);//close file


   	fp = fopen("/var/racoon.conf", "a+");
  	 	if (fp == NULL) {
     	 printf("app/ipsec: unable to open racoon file\n");
     	 return ;
  		 }
   		fprintf(fp, "path pre_shared_key \"/var/psk.txt\";\n");
   		fprintf(fp, "path certificate \"%s\";\n\n", CERT_PATH);
   		
   		fprintf(fp, "timer\n{\n\tcounter 5;\n\tphase1 15 sec;\n\tphase2 15 sec;\n}\n");
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

		//get localMask
		if((getNthValueSafe(6, rec, ',', localMask, sizeof(localMask)) == -1)){
			continue;
		}

		//get remoteIP
		if((getNthValueSafe(7, rec, ',', remoteIP, sizeof(remoteIP)) == -1)){
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
		//get nat_traversal
		if((getNthValueSafe(29, rec, ',', nat_traversal, sizeof(nat_traversal)) == -1)){
			continue;
		}
		 if((getNthValueSafe(30, rec, ',', policies_remote_lan, sizeof(policies_remote_lan)) == -1)){
                        continue;
                }
		 

		
		if(states[1] == '0')   //关闭状态不生成配置文件
		{
			continue;
		}
		else{
			sign_havelink=2;//add by zsf 2010-10-30

			if (!strcmp(localIPMode, "subnet")){
				 local_prefixlen = rutIPSec_calPrefixLen(localMask);
			}else if(!strcmp(localIPMode, "any")){
					local_prefixlen = 0;
					strcpy(localIP,"0.0.0.0");
			} else{
					local_prefixlen = 32;
			}

			 if (!strcmp(remoteIPMode, "subnet")){
				 remote_prefixlen = rutIPSec_calPrefixLen(remoteMask);
			 }else if(!strcmp(remoteIPMode, "any")){
			 	remote_prefixlen=0;
				strcpy(remoteIP,"0.0.0.0");
			  }else{
					remote_prefixlen = 32;
			 }


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
		
			 //not add for any
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

		//		if(!((local_prefixlen==0)||(remote_prefixlen==0))){
     			//	sprintf(cmd, "iptables -t nat -I POSTROUTING 1 -s %s/%d -o %s -d %s/%d -j ACCEPT",
                   	//		localIP, local_prefixlen, getWanIfNamePPP(), remoteIP, remote_prefixlen);
      			//			system(cmd);
		//		}




				sprintf(cmd, "iptables -t nat -D POSTROUTING -s %s/%d -o %s -d 0.0.0.0/0 -j ACCEPT",
                                              localIP, local_prefixlen, getWanIfNamePPP());
                                system(cmd);

                                if((local_prefixlen!=0)&&(remote_prefixlen==0)&&(strlen(policies_remote_lan)>8)){ //remote any mode
                                        lan_count=0;
                                        memset(lan_mask_one,0,sizeof(lan_mask_one));
                                        while(getNthValueSafe(lan_count++,policies_remote_lan, ' ',lan_mask_one,sizeof(lan_mask_one)) != -1 ){
                                                sprintf(cmd, "iptables -t nat -I POSTROUTING 1 -s %s/%d -o %s -d %s -j ACCEPT",
                                                               localIP, local_prefixlen, getWanIfNamePPP(), lan_mask_one);
                                                system(cmd);
                                                memset(lan_mask_one,0,sizeof(lan_mask_one));
                                        }

                                }else{
                                        sprintf(cmd, "iptables -t nat -I POSTROUTING 1 -s %s/%d -o %s -d %s/%d -j ACCEPT",
                                                localIP, local_prefixlen, getWanIfNamePPP(), remoteIP, remote_prefixlen);
                                        system(cmd);

                                }




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
                fprintf(fp, "script \"/sbin/ipsec_vpn_led.sh\" phase1_up;\n");
                fprintf(fp, "script \"/sbin/ipsec_vpn_led.sh\" phase1_down;\n");
                fprintf(fp, "dpd_delay 60;\n");
                fprintf(fp, "dpd_maxfail 5;\n");
       	  fprintf(fp, "exchange_mode %s;\n", ph1Mode);
		  if (!strcmp(authM, "pre_shared_key")) {
		  fprintf(fp, "my_identifier fqdn \"%s\";\n", connName);
		  	}



	if(strstr(nat_traversal,"on")){
         	fprintf(fp, "nat_traversal on;\n");
	}
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
                fprintf(fp, "script \"/sbin/ipsec_vpn_led.sh\" phase1_up;\n");
                fprintf(fp, "script \"/sbin/ipsec_vpn_led.sh\" phase1_down;\n");
                fprintf(fp, "dpd_delay 60;\n");
                fprintf(fp, "dpd_maxfail 5;\n");
       	   	 fprintf(fp, "exchange_mode aggressive,main;\n");
			  if (!strcmp(authM, "pre_shared_key")) {
	//		fprintf(fp, "peers_identifier fqdn \"%s\";\n", connName);
			  	}
			fprintf(fp, "generate_policy on;\n");
			fprintf(fp, "passive on;\n");
		


	if(strstr(nat_traversal,"on")){
         	fprintf(fp, "nat_traversal on;\n");
	}
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





			
	}
	

	
	if((local_prefixlen==0)&&(remote_prefixlen==0)){
         	fprintf(fp, "sainfo anonymous {\n");
	}else if(local_prefixlen==0){
         	fprintf(fp, "sainfo anonymous  address %s/%d any {\n",remoteIP, remote_prefixlen);
	}else if(remote_prefixlen==0){
         	fprintf(fp, "sainfo address %s/%d any anonymous {\n",localIP, local_prefixlen);
	}else{
         	fprintf(fp, "sainfo address %s/%d any address %s/%d any {\n",localIP, local_prefixlen, remoteIP, remote_prefixlen);
	}	

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

	if(sign_havelink==1){
		return;
	}

	system("chmod 600 /var/psk.txt");
	system("chmod 640 /var/setkey.conf");
	system("chmod 640 /var/racoon.conf");
    system("racoonctl flush-sa ipsec");
	system("killall racoon");
	system("setkey -f var/setkey.conf");
	system("racoon -f var/racoon.conf");

	sleep(5);
	system("ping_ipsec.sh&");
    system("kill_pingany.sh&");//kill ping_any 
     system("kill_pingany.sh&");//kill ping_any 
    sleep(2);
      system("ping_any.sh&");//
	

	sign_link_up=1;
   // nvram_bufset(RT2860_NVRAM, "VpnLinkSign","linkup");

	return;

}
















void dns_flash_ip(void){

  

	

char* PPP_Name;
	int have_server_remote_flag=0;
	static int flag = 0 ;
	FILE *fp;
   	char wanIP[32];
   	int local_prefixlen;
   	int remote_prefixlen;
  	char cmd[256];
	int i = 0;
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
	char *rules = nvram_bufget(RT2860_NVRAM, "IPSECRules");

	char *lan_ip=nvram_bufget(RT2860_NVRAM, "lan_ipaddr");
 	char* lan_mask=nvram_bufget(RT2860_NVRAM, "lan_netmask");
	int lan_prefix;

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
	PPP_Name=getWanIfNamePPP();
	if (-1 == getIfIp(PPP_Name, wanIP))
		{
		 //do nothing
	//		printf("Can't get Wan IP\n");
		 	return;
		}
	//printf("wanIP==%s\n",wanIP);
	//printf("getWanIfNamePPP==%s\n",getWanIfNamePPP());
  




	system("rm -rf /var/racoon.conf");
	system("rm -rf /var/setkey.conf");
	system("rm -rf /var/psk.txt");	







	//创建配置文件开始部分

	fp = fopen("/var/setkey.conf", "a+");
			
		 if (fp == NULL) {      
				printf("app/ipsec: unable to open setkey file\n");      
				return ;   
		}  
		fprintf(fp, "flush;\n");   
		fprintf(fp, "spdflush;\n");

		lan_prefix=rutIPSec_calPrefixLen(lan_mask);
		 fprintf(fp, "spdadd %s/%d %s/%d any -P out none;\n", lan_ip, lan_prefix, lan_ip, lan_prefix);
		 fprintf(fp, "spdadd %s/%d %s/%d any -P in none;\n", lan_ip, lan_prefix, lan_ip, lan_prefix);	     		  			 

		
		fclose(fp);//close file


   	fp = fopen("/var/racoon.conf", "a+");
  	 	if (fp == NULL) {
     	 printf("app/ipsec: unable to open racoon file\n");
     	 return ;
  		 }
   		fprintf(fp, "path pre_shared_key \"/var/psk.txt\";\n");
   		fprintf(fp, "path certificate \"%s\";\n\n", CERT_PATH);
   		
   		fprintf(fp, "timer\n{\n\tcounter 5;\n\tphase1 15 sec;\n\tphase2 15 sec;\n}\n");
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

			
		}

		// get localIPMode
		if((getNthValueSafe(4, rec, ',', localIPMode, sizeof(localIPMode)) == -1)){
			continue;
		}

		//get localIP
		if((getNthValueSafe(5, rec, ',', localIP, sizeof(localIP)) == -1)){
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
		//get ph2KeyTime
		if((getNthValueSafe(29, rec, ',', nat_traversal, sizeof(nat_traversal)) == -1)){
			continue;
		}
		if((getNthValueSafe(30, rec, ',', policies_remote_lan, sizeof(policies_remote_lan)) == -1)){
                        continue;
                }
		 








		
		if(states[1] == '0')   //关闭状态不生成配置文件
		{
			continue;
		}
		else{


			if (!strcmp(localIPMode, "subnet")){
				 local_prefixlen = rutIPSec_calPrefixLen(localMask);
			}else if(!strcmp(localIPMode, "any")){
					local_prefixlen = 0;
					strcpy(localIP,"0.0.0.0");
			} else{
					local_prefixlen = 32;
			}



				
			 if (!strcmp(remoteIPMode, "subnet")){
				 remote_prefixlen = rutIPSec_calPrefixLen(remoteMask);
			 }else if(!strcmp(remoteIPMode, "any")){
			 	remote_prefixlen=0;
				strcpy(remoteIP,"0.0.0.0");
			  }else{
					remote_prefixlen = 32;
			 }




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
				//if(!((local_prefixlen==0)||(remote_prefixlen==0))){
					//do not used this iptables 	
     					//sprintf(cmd, "iptables -t nat -I POSTROUTING 1 -s %s/%d -o %s -d %s/%d -j ACCEPT",
                   			//	localIP, local_prefixlen, getWanIfNamePPP(), remoteIP, remote_prefixlen);
      					//		system(cmd);
				//}

      			//printf ("ipsec:spdadd Done\n");


			 //racoon

			
      	if (!strcmp(keyExM, "auto") ){


		fp = fopen("/var/racoon.conf", "a+");
  	 		if (fp == NULL) {
     		 printf("app/ipsec: unable to open racoon file\n");
     		 return ;
  			 }








	if (!strcmp(service_mod_sel, "client")) {
	      	   fprintf(fp, "remote %s {\n", remoteGWAddr);
                fprintf(fp, "script \"/sbin/ipsec_vpn_led.sh\" phase1_up;\n");
                fprintf(fp, "script \"/sbin/ipsec_vpn_led.sh\" phase1_down;\n");
                fprintf(fp, "dpd_delay 60;\n");
                fprintf(fp, "dpd_maxfail 5;\n");
       	  fprintf(fp, "exchange_mode %s;\n", ph1Mode);
		  if (!strcmp(authM, "pre_shared_key")) {
		  fprintf(fp, "my_identifier fqdn \"%s\";\n", connName);
		  	}




	if(strstr(nat_traversal,"on")){
         	fprintf(fp, "nat_traversal on;\n");
	}
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
                fprintf(fp, "script \"/sbin/ipsec_vpn_led.sh\" phase1_up;\n");
                fprintf(fp, "script \"/sbin/ipsec_vpn_led.sh\" phase1_down;\n");
                fprintf(fp, "dpd_delay 60;\n");
                fprintf(fp, "dpd_maxfail 5;\n");
       	   	 fprintf(fp, "exchange_mode aggressive,main;\n");
			  if (!strcmp(authM, "pre_shared_key")) {
	//		fprintf(fp, "peers_identifier fqdn \"%s\";\n", connName);
			  	}
			fprintf(fp, "generate_policy on;\n");
			fprintf(fp, "passive on;\n");
		

	if(strstr(nat_traversal,"on")){
         	fprintf(fp, "nat_traversal on;\n");
	}

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





			
	}
	

/*	

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
*/


	if((local_prefixlen==0)&&(remote_prefixlen==0)){
         	fprintf(fp, "sainfo anonymous {\n");
	}else if(local_prefixlen==0){
         	fprintf(fp, "sainfo anonymous  address %s/%d any {\n",remoteIP, remote_prefixlen);
	}else if(remote_prefixlen==0){
         	fprintf(fp, "sainfo address %s/%d any anonymous {\n",localIP, local_prefixlen);
	}else{
         	fprintf(fp, "sainfo address %s/%d any address %s/%d any {\n",localIP, local_prefixlen, remoteIP, remote_prefixlen);
	}	

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
			
/*	
		if (1 != flag){
		   system("racoon -f var/racoon.conf");
           system("setkey -f var/setkey.conf");
		   flag = 1 ;
		}
*/
		}
	
	}


	system("chmod 600 /var/psk.txt");
	system("chmod 640 /var/setkey.conf");
	system("chmod 640 /var/racoon.conf");


	/*
	system("killall racoon");
	system("setkey -f var/setkey.conf");
	system("racoon -f var/racoon.conf");
	
	sign_link_up=1;
	*/
   // nvram_bufset(RT2860_NVRAM, "VpnLinkSign","linkup");

	return;

}



/*

int get_dns_change_sign(void)
{
	FILE* fp;
	char buf[20];
	if((fp=popen("vpn_dns_ip_check_for_restart get_change_flag","r"))==NULL){
		return 0;	
	}
	memset(buf,0,sizeof(buf));	
	fgets(buf,sizeof(buf),fp);
	if(fp)	pclose(fp);

	if(strstr(buf,"changed")) return 1;
	else return 0;
}
*/
int main(int argc, char** argv)
{
    char *vpn_link_sign;
	int dns_change_sign=0;
	int ping_ipsec_count=0;

   /* int i;
    struct sigaction s;
    s.sa_flags = SA_SIGINFO;
    s.sa_sigaction = (void *)myhandler;
    for(i = 1; i < 32; i++)
    {
        if(sigaction (i,&s,(struct sigaction *)NULL)) {
            printf("Sigaction returned error = %d\n", errno);
            //exit(0);
        }
    }*/



    /* ignore SIGPIPE (send if transmitting to closed sockets) */
    signal(SIGPIPE, SIG_IGN);
signal(SIGUSR1, SIG_IGN);
signal(SIGUSR2, SIG_IGN);


  //  nvram_bufset(RT2860_NVRAM, "VpnLinkSign","linkdown");

	system("vpn_dns_ip_check_for_restart reset");
  
	while(1)
	{
	//	vpn_link_sign=nvram_bufget(RT2860_NVRAM,"VpnLinkSign");
	//	if(strstr(vpn_link_sign,"linkdown"))
		if(sign_link_up==0)
		{
			ipsec_run();
			if(sign_havelink==1){
				return 0; //exit
			}
		}
/*
        else{

             system("echo \"success\">>/var/vpn.log");
			//exit(0);	
	//		dns_change_sign=get_dns_change_sign();
	//		if(dns_change_sign==1)
			dns_flash_ip();
			system("vpn_dns_ip_check_for_restart");

			if(ping_ipsec_count++>5){ //60 sec
				ping_ipsec_count=0;
				system("ping_ipsec.sh &"); //to make ipsec link
				sleep(5);
			}
			
		
		}
*/
		sleep(10);
	}

    return 0;
	
}

