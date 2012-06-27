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

#include "vrrp_action.h"

#define nvram_vrrp_para "vrrp_para"


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
	nvram_bufset(RT2860_NVRAM, "vrrp_active",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,1,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "vrrp_id",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,2,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "vrrp_priority",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,3,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "vrrp_interval",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,4,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "vrrp_password",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,5,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "vrrp_virtual_ip",str_tmp);


      memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,6,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "vrrp_preemption_mode",str_tmp);


   return true;

   }


     int load_vrrp_para_to_buf(void)
   {
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_vrrp_para);
	if(para==NULL){
		printf("read vrrp para false!");
		return false;
	}
	if(setbuf_one_para(para)==false){
		printf("vrrp init para false!\n");
		return false;
	}
	printf("vrrp init para succeed!\n");
	return true;
}




   void vrrp_run(int sign)
  {
	if(sign==1){
		doSystem("start_vrrp.sh&");
	}else{
		doSystem("killall keepalived");
	}
  }




   static void vrrp_action(webs_t wp, char_t *path, char_t *query)
  {
  	char vrrp_para[1024];
	char *vrrp_active;
	char*vrrp_id;
	char *vrrp_priority;
	char *vrrp_interval;
	char *vrrp_password;
	char *vrrp_virtual_ip;
	char *vrrp_preemption_mode;




	
	int run_sign=0;
	
	
		vrrp_active=websGetVar(wp,T("vrrp_active") , T(""));
			
             if(strstr(vrrp_active,"on"))  {
			 
                            run_sign=1;
		}else{
                             run_sign=0;
		}
		nvram_bufset(RT2860_NVRAM, "vrrp_active",vrrp_active);


              vrrp_id=websGetVar(wp,T("vrrp_id"), T(""));
		nvram_bufset(RT2860_NVRAM, "vrrp_id",vrrp_id);


		vrrp_priority= websGetVar(wp, T("vrrp_priority"), T(""));
		nvram_bufset(RT2860_NVRAM, "vrrp_priority",vrrp_priority);
		
		vrrp_interval= websGetVar(wp, T("vrrp_interval"), T(""));
		nvram_bufset(RT2860_NVRAM, "vrrp_interval",vrrp_interval);

		vrrp_password= websGetVar(wp, T("vrrp_password"), T(""));											
		nvram_bufset(RT2860_NVRAM, "vrrp_password",vrrp_password);


		vrrp_virtual_ip= websGetVar(wp, T("vrrp_virtual_ip"), T(""));	
		nvram_bufset(RT2860_NVRAM, "vrrp_virtual_ip",vrrp_virtual_ip);
		
		
		vrrp_preemption_mode= websGetVar(wp, T("vrrp_preemption_mode"), T(""));	
		nvram_bufset(RT2860_NVRAM, "vrrp_preemption_mode",vrrp_preemption_mode);


			memset(vrrp_para,0,sizeof(vrrp_para));
			sprintf(vrrp_para,"%s,%s,%s,%s,%s,%s,%s",vrrp_active,vrrp_id,vrrp_priority,vrrp_interval,vrrp_password, vrrp_virtual_ip,vrrp_preemption_mode);







	//printf("rulesbuf:%s\n", rulesbuf);
	nvram_set(RT2860_NVRAM,nvram_vrrp_para, vrrp_para);
	nvram_commit(RT2860_NVRAM);


//	load_fail_over_para_to_buf();

	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "internet/vrrp.asp");



//ipsec_run(); 
	vrrp_run(run_sign);



	return;

		
}

			
void formDefinelvrrp(void)
{
	websFormDefine(T("vrrp_action"), vrrp_action);
	
}


void init_vrrp(void)
{
	formDefinelvrrp();
	load_vrrp_para_to_buf();			  
}

