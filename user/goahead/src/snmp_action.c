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

#include "snmp_action.h"

#define nvram_snmp_para "snmp_para"


#define true 1
#define false 0



static int locate_para(char*para,int num,char ch,char* ret)
{
	char *p,*p1;
	int i;
	if((para==NULL)||(ret==NULL)) return false;
	p=para;
	//p1=para;

	
	for(i=0;i<num;i++){
		p=strchr(p,ch);
		if(p==NULL) return false;
		p++;
	}
	if((p1=strchr(p,ch))==NULL) {
		strcpy(ret,p);		
	}else{
		strncpy(ret,p,p1-p);
	}

	return true;
}


static int setbuf_one_para(char* para)
{		
	//FILE* fp;
//	char*p,*p1;
	int ret;
	char str_tmp[256];

	
	if(para==NULL) return false;

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,0,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "snmp_status",str_tmp);


		//fprintf(fp,"snmp_serverport:%s\n",str_tmp);

	//fseek(fp,0,SEEK_SET);
	//fprintf(fp,"make config file succeed!",str_tmp);
	//fclose(fp);


	return true;

	
}



int load_snmp_para_to_buf(void)
{
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_snmp_para);
	if(para==NULL){
		printf("read snmp_para false!");
		return false;
	}
	if(setbuf_one_para(para)==false){
		printf(" snmp init para false!\n");
		return false;
	}
	printf(" snmp init para succeed!\n");
	return true;
}



void snmp_run(int sign)
{
	if(sign==1){
		doSystem("start_snmp.sh&");
	}else{
		doSystem("killall snmp");

	}
}








static void snmp_action(webs_t wp, char_t *path, char_t *query)
{
	char snmp_para[1024];
	char *snmp_status,*snmp_contact_info,*snmp_location,*snmp_v2_user,*snmp_v2_host_lan,*snmp_v2_writable;
	char *snmp_v3_user,*snmp_v3_host_lan,*snmp_v3_writable,*snmp_v3_security_mode,*snmp_v3_authentication;
	char*snmp_v3_encryption,*snmp_v3_authentication_password,*snmp_v3_encryption_password;


	
	int run_sign=0;
	
	
	 	snmp_status = websGetVar(wp, T("snmp_active"), T(""));
		if(strstr(snmp_status,"on")) run_sign=1;
		else run_sign=0;
		nvram_bufset(RT2860_NVRAM, "snmp_active",snmp_status);
		
		snmp_contact_info = websGetVar(wp, T("snmp_contact_info"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_contact_info",snmp_contact_info);
		
		snmp_location = websGetVar(wp, T("snmp_location"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_location",snmp_location);		


		snmp_v2_user = websGetVar(wp, T("snmp_v2_user"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v2_user",snmp_v2_user);		
		snmp_v2_host_lan = websGetVar(wp, T("snmp_v2_host_lan"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v2_host_lan",snmp_v2_host_lan);		
		snmp_v2_writable = websGetVar(wp, T("snmp_v2_writable"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v2_writable",snmp_v2_writable);		
		snmp_v3_user = websGetVar(wp, T("snmp_v3_user"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v3_user",snmp_v3_user);		
		snmp_v3_host_lan = websGetVar(wp, T("snmp_v3_host_lan"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v3_host_lan",snmp_v3_host_lan);		
		snmp_v3_writable = websGetVar(wp, T("snmp_v3_writable"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v3_writable",snmp_v3_writable);		
		snmp_v3_security_mode = websGetVar(wp, T("snmp_v3_security_mode"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v3_security_mode",snmp_v3_security_mode);		
		snmp_v3_authentication = websGetVar(wp, T("snmp_v3_authentication"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v3_authentication",snmp_v3_authentication);		
		snmp_v3_encryption = websGetVar(wp, T("snmp_v3_encryption"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v3_encryption",snmp_v3_encryption);		

		snmp_v3_authentication_password = websGetVar(wp, T("snmp_v3_authentication_password"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v3_authentication_password",snmp_v3_authentication_password);		
		snmp_v3_encryption_password = websGetVar(wp, T("snmp_v3_encryption_password"), T(""));
		nvram_bufset(RT2860_NVRAM, "snmp_v3_encryption_password",snmp_v3_encryption_password);		

			
		memset(snmp_para,0,sizeof(snmp_para));
		snprintf(snmp_para,sizeof(snmp_para),"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
					snmp_status,snmp_contact_info,snmp_location,snmp_v2_user,snmp_v2_host_lan,snmp_v2_writable,
					snmp_v3_user,snmp_v3_host_lan,snmp_v3_writable,snmp_v3_security_mode,snmp_v3_authentication,
					snmp_v3_encryption,snmp_v3_authentication_password,snmp_v3_encryption_password
					);


	nvram_set(RT2860_NVRAM, nvram_snmp_para, snmp_para);
	nvram_commit(RT2860_NVRAM);


//	load_gps_para_to_buf();

	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "internet/snmp.asp");



//ipsec_run(); 
	snmp_run(run_sign);



	return;

		
}








void formDefineSNMP(void)
{
	websFormDefine(T("snmp_action"), snmp_action);
	
}


void init_snmp(void)
{
	formDefineSNMP();
	load_snmp_para_to_buf();
	
}


