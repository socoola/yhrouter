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

#include "reboot_action.h"

#define nvram_reboot_para "reboot_para"
#define nvram_network_reboot_para "network_reboot_para"

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
	nvram_bufset(RT2860_NVRAM, "rb_when_timeout",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,1,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_timer",str_tmp);


   return true;

   }

static int rb_network_setbuf_one_para(char* para)
{		
	//FILE* fp;
//	char*p,*p1;
	int ret;
	char str_tmp[1024];



	
	
	if(para==NULL) return false;

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,0,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_when_network_error",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,1,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_network_check_method",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,2,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_network_check_host",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,3,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_network_check_host_2",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,4,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_network_check_interval_time",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,5,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_network_checkcount",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,6,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_network_sleep_count",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,7,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "rb_network_sleep_time",str_tmp);


   return true;

   }
     int load_rb_para_to_buf(void)
   {
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_reboot_para);
	if(para==NULL){
		printf("read reboot para false!");
		return false;
	}
	if(setbuf_one_para(para)==false){
		printf("reboot init para false!\n");
		return false;
	}
	printf("reboot init para succeed!\n");
	return true;
}

     int load_network_rb_para_to_buf(void)
   {
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_network_reboot_para);
	if(para==NULL){
		printf("read network error reboot para false!");
		return false;
	}
	if(rb_network_setbuf_one_para(para)==false){
		printf("network error reboot init para false!\n");
		return false;
	}
	printf("network error reboot init para succeed!\n");
	return true;
}


   void reboot_timer_run(int sign)
  {
	if(sign==1){
		doSystem("start_reboot_timer.sh start &");
	}else{
		doSystem("start_reboot_timer.sh stop &");
	}
  }


   void network_reboot_run(int sign)
  {
	if(sign==1){
		doSystem("start_reboot_timer.sh network_reboot_start &");
	}else{
		doSystem("start_reboot_timer.sh network_reboot_stop &");
	}
  }


 static void rb_network_action(webs_t wp, char_t *path, char_t *query)
{
	char rb_network_para[1024];
	char *rb_when_network_error;
	char *rb_network_check_method;
	char*rb_network_check_host;
	char*rb_network_check_host_2;
	char*rb_network_checkcount;
	char*rb_network_check_interval_time;
	char*rb_network_sleep_count;
	char*rb_network_sleep_time;
	int run_sign=0;


	char*button;



	char*hostip;	
	FILE*fp;
	char buf[256];
	char buf_ret[256];

	button=websGetVar(wp,T("rb_button_sign") , T(""));
	if(strstr(button,"check")){


			memset(buf,0,sizeof(buf));
			memset(buf_ret,0,sizeof(buf_ret));

			if(strstr(button,"check_2")){
				hostip=websGetVar(wp,T("rb_network_check_host_2") , T(""));
			}else{
				hostip=websGetVar(wp,T("rb_network_check_host") , T(""));
			}
			
			if(strlen(hostip)<4){
				 websWrite(wp, T("<h2>the Host/Ip is Error!</h2>\n"));
			}else{


				strcpy(buf,"reboot_network_check_function.sh ping_host ");
				strcat(buf,hostip);

				fp=popen(buf,"r");
				if(fp==NULL){
					 websWrite(wp, T("<h2>the Host/Ip is Error!</h2>\n"));
		
				}else{

					fread(buf_ret,sizeof(char),sizeof(buf_ret),fp);
					if(fp) pclose(fp);

					if(strstr(buf_ret,"OK")){
	
						rb_network_check_host=websGetVar(wp,T("rb_network_check_host") , T(""));
						nvram_bufset(RT2860_NVRAM, "rb_network_check_host",rb_network_check_host);
						 websWrite(wp, T("<h2>the Host/Ip is OK!</h2>\n"));
					}else{

						 websWrite(wp, T("<h2>the Host/Ip is Error!</h2>\n"));
					}

				}


			}

			return;

	}else{
	
			rb_when_network_error=websGetVar(wp,T("rb_when_network_error") , T(""));
			nvram_bufset(RT2860_NVRAM, "rb_when_network_error",rb_when_network_error);
		             if(strstr(rb_when_network_error,"on"))  {
					 
		                            run_sign=1;
				}else{
		                             run_sign=0;
				}


			
			rb_network_check_method=websGetVar(wp,T("rb_network_ping_sel") , T(""));
			nvram_bufset(RT2860_NVRAM, "rb_network_check_method",rb_network_check_method);
			
			rb_network_check_host=websGetVar(wp,T("rb_network_check_host") , T(""));
			nvram_bufset(RT2860_NVRAM, "rb_network_check_host",rb_network_check_host);

			rb_network_check_host_2=websGetVar(wp,T("rb_network_check_host_2") , T(""));
			nvram_bufset(RT2860_NVRAM, "rb_network_check_host_2",rb_network_check_host_2);

			rb_network_check_interval_time=websGetVar(wp,T("rb_network_check_interval_time") , T(""));
			nvram_bufset(RT2860_NVRAM, "rb_network_check_interval_time",rb_network_check_interval_time);

			rb_network_checkcount=websGetVar(wp,T("rb_network_checkcount") , T(""));
			nvram_bufset(RT2860_NVRAM, "rb_network_checkcount",rb_network_checkcount);

			rb_network_sleep_count=websGetVar(wp,T("rb_network_sleep_count") , T(""));
			nvram_bufset(RT2860_NVRAM, "rb_network_sleep_count",rb_network_sleep_count);

			rb_network_sleep_time=websGetVar(wp,T("rb_network_sleep_time") , T(""));
			nvram_bufset(RT2860_NVRAM, "rb_network_sleep_time",rb_network_sleep_time);




					memset(rb_network_para,0,sizeof(rb_network_para));
					sprintf(rb_network_para,"%s,%s,%s,%s,%s,%s,%s,%s",rb_when_network_error,rb_network_check_method,
						rb_network_check_host,rb_network_check_host_2,rb_network_check_interval_time,rb_network_checkcount,rb_network_sleep_count,rb_network_sleep_time);

			nvram_set(RT2860_NVRAM,nvram_network_reboot_para, rb_network_para);
			nvram_commit(RT2860_NVRAM);



			/* 完成转到ipsec 配置页面*/
			websRedirect(wp, "adm/reboot.asp");


			network_reboot_run(run_sign);	

	}

	return;
	
}
   static void rb_timer_action(webs_t wp, char_t *path, char_t *query)
  {
  	char rb_para[256];
	char *rb_when_timeout;
	char*rb_timer;




	
	int run_sign=0;
	
	
		rb_when_timeout=websGetVar(wp,T("rb_when_timeout") , T(""));
			
             if(strstr(rb_when_timeout,"on"))  {
			 
                            run_sign=1;
		}else{
                             run_sign=0;
		}
		nvram_bufset(RT2860_NVRAM, "rb_when_timeout",rb_when_timeout);


		rb_timer= websGetVar(wp, T("rb_timer"), T(""));											
		nvram_bufset(RT2860_NVRAM, "rb_timer",rb_timer);


			memset(rb_para,0,sizeof(rb_para));
			sprintf(rb_para,"%s,%s",rb_when_timeout,rb_timer);

	nvram_set(RT2860_NVRAM,nvram_reboot_para, rb_para);
	nvram_commit(RT2860_NVRAM);



	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "adm/reboot.asp");




	reboot_timer_run(run_sign);



	return;

		
}

/*
static int check_host_ip(int eid,webs_t wp,int argc,char_t **argv)
{
	char_t*hostip;	
	FILE*fp;
	char buf[256];
	char buf_ret[256];

	memset(buf,0,sizeof(buf));
	memset(buf_ret,0,sizeof(buf_ret));
	
	if(ejArgs(argc,argv,T("%s"),&hostip)<1){
		return	websWrite(wp,"ERROR");	
	}

	if(strlen((char*)hostip)<4){
		return	websWrite(wp,"ERROR");	
	}


	strcpy(buf,"reboot_network_check_function.sh ping_host  ");
	strcat(buf,(char*)hostip);
	fp=popen(buf,"r");
	if(fp==NULL){
		return	websWrite(wp,"ERROR");	
		
	}
	fread(buf_ret,sizeof(char),sizeof(buf_ret),fp);
	if(fp) pclose(fp);

	if(strstr(buf_ret,"OK")){
		return	websWrite(wp,"OK");	
	}else{
		return	websWrite(wp,"ERROR");	
	}
}
*/
/*
static void check_host_ip(void)
{
	char*hostip;	
	FILE*fp;
	char buf[256];
	char buf_ret[256];

	memset(buf,0,sizeof(buf));
	memset(buf_ret,0,sizeof(buf_ret));
	
	hostip=websGetVar(wp,T("rb_network_check_host") , T(""));

	if(strlen(hostip)<4){
		//return	websWrite(wp,"ERROR");	
		return websWrite(wp, T("<h2>the Host/Ip is Error!</h2><br>\n"));
	}


	strcpy(buf,"reboot_network_check_function.sh ping_host  ");
	strcat(buf,hostip);
	fp=popen(buf,"r");
	if(fp==NULL){
	//	return	websWrite(wp,"ERROR");	
		return websWrite(wp, T("<h2>the Host/Ip is Error!</h2><br>\n"));
		
	}
	fread(buf_ret,sizeof(char),sizeof(buf_ret),fp);
	if(fp) pclose(fp);

	if(strstr(buf_ret,"OK")){
	//	return	websWrite(wp,"OK");	
		return websWrite(wp, T("<h2>the Host/Ip is OK!</h2><br>\n"));
	}else{
	//	return	websWrite(wp,"ERROR");	
		return websWrite(wp, T("<h2>the Host/Ip is Error!</h2><br>\n"));
	}
}
*/
static void rb_action(webs_t wp, char_t *path, char_t *query)
{
	doSystem("start_reboot_timer.sh now &");
	return;
}










			
void formDefinereboot(void)
{
	websFormDefine(T("rb_timer_action"), rb_timer_action);
	websFormDefine(T("rb_action"), rb_action);
	websFormDefine(T("rb_network_action"), rb_network_action);


	//websAspDefine(T("check_host_ip"), check_host_ip);
}


void init_reboot(void)
{
	formDefinereboot();
	load_rb_para_to_buf();	
	load_network_rb_para_to_buf();
}

