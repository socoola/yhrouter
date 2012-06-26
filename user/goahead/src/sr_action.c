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

#include "sr_action.h"

#define nvram_sr_para "sr_para"


#define true 1
#define false 0



static  int locate_para(char*para,int num,char ch,char* ret)
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
	int ret;
	char str_tmp[256];

	if(para==NULL) return false;

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,0,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "sr_status",str_tmp);
	//fprintf(fp,"dtu_status:%s\n",str_tmp);



	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,1,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "sr_test_host",str_tmp);
	//fprintf(fp,"dtu_bandrate:%s\n",str_tmp);

	

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,2,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "sr_report_host",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,3,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "sr_report_port",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,4,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "sr_report_time",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);





		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,5,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "sr_report_status1_str",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,6,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "sr_report_status2_str",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);




	return true;

	
}



int load_sr_para_to_buf(void)
{
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_sr_para);
	if(para==NULL){
		printf("read status report para false!");
		return false;
	}
	if(setbuf_one_para(para)==false){
		printf(" status report  init para false!\n");
		return false;
	}
	printf(" status report  init para succeed!\n");
	return true;
}



void sr_run(int sign)
{
	if(sign==1){
		doSystem("killall status_report");
		doSystem("start_status_report&");
	}else{
		doSystem("killall status_report");
	}
}








static void sr_action(webs_t wp, char_t *path, char_t *query)
{
	int run_sign=0;
	char sr_para[1024];
	char *sr_status;
	char *sr_test_host;
	char *sr_report_host;
	char *sr_report_port;
	char *sr_report_time;
	char *sr_report_status1_str;
	char *sr_report_status2_str;	
	



	
	 	sr_status = websGetVar(wp, T("sr_status_sel"), T(""));
		if(strstr(sr_status,"off"))	run_sign=0;
		else 	run_sign=1;
		nvram_bufset(RT2860_NVRAM, "sr_status",sr_status);

		
		sr_test_host = websGetVar(wp, T("sr_test_host"), T(""));
		nvram_bufset(RT2860_NVRAM, "sr_test_host",sr_test_host);
		
		sr_report_host = websGetVar(wp, T("sr_report_host"), T(""));
		nvram_bufset(RT2860_NVRAM, "sr_report_host",sr_report_host);
		
		sr_report_port = websGetVar(wp, T("sr_report_port"), T(""));
		nvram_bufset(RT2860_NVRAM, "sr_report_port",sr_report_port);
		
		sr_report_time = websGetVar(wp, T("sr_report_time"), T(""));
		nvram_bufset(RT2860_NVRAM, "sr_report_time",sr_report_time);


		sr_report_status1_str = websGetVar(wp, T("sr_report_status1_str"), T(""));
		nvram_bufset(RT2860_NVRAM, "sr_report_status1_str",sr_report_status1_str);
		
		sr_report_status2_str = websGetVar(wp, T("sr_report_status2_str"), T(""));
		nvram_bufset(RT2860_NVRAM, "sr_report_status2_str",sr_report_status2_str);
		


			memset(sr_para,0,sizeof(sr_para));
			snprintf(sr_para,sizeof(sr_para),"%s,%s,%s,%s,%s,%s,%s",
					sr_status,sr_test_host,sr_report_host,sr_report_port,sr_report_time,sr_report_status1_str,sr_report_status2_str
					);








	nvram_set(RT2860_NVRAM, nvram_sr_para, sr_para);
	nvram_commit(RT2860_NVRAM);


//	load_dtu_para_to_buf();

	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "internet/status_report.asp");



//ipsec_run(); 
	sr_run(run_sign);



	return;

		
}








void formDefine_STATUS_REPORT(void)
{
	websFormDefine(T("sr_action"), sr_action);
}


void init_status_report(void)
{
	formDefine_STATUS_REPORT();
	load_sr_para_to_buf();
	
}


