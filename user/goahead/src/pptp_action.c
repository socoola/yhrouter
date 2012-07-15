#include	<stdlib.h>
//#include	<sys/ioctl.h>
//#include	<net/if.h>
//#include	<net/route.h>
#include    <string.h>
//#include    <dirent.h>
//#include 	<winsock.h>

#include	"internet.h"
#include	"nvram.h"
#include	"webs.h"
#include	"utils.h"
//#include 	"firewall.h"

//#include	"linux/autoconf.h"  //kernel config
//#include	"config/autoconf.h" //user config
//#include	"user/busybox/include/autoconf.h" //busybox config

#include "pptp_action.h"

#define nvram_pptp_para "pptp_para"


#define true 1
#define false 0


/*

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
	nvram_bufset(RT2860_NVRAM, "link_backup_status",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,1,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "link_backup_backtomain",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,2,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "link_backup_first",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,3,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "link_backup_second",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,4,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "link_backup_checkcount",str_tmp);


	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,5,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "link_backup_ping",str_tmp);


      memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,6,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "link_backup_host",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,7,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "link_backup_wan2_ping",str_tmp);


      memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,8,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "link_backup_wan2_host",str_tmp);



   return true;

   }


     int load_pptp_para_to_buf(void)
   {
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_pptp_para);
	if(para==NULL){
		printf("read linkbackup para false!");
		return false;
	}
	if(setbuf_one_para(para)==false){
		printf("linkbackup init para false!\n");
		return false;
	}
	printf("linkbackup init para succeed!\n");
	return true;
}


*/

void pptp_run(int sign)
{
	if(sign==1){
		doSystem("pptp_oper.sh start &");
	}else{
		doSystem("pptp_oper.sh stop &");
	}
}




   static void pptp_action_1(webs_t wp, char_t *path, char_t *query)
{
	fprintf(stderr,"action1 ok\n");
	return;
}
   static void pptp_action(webs_t wp, char_t *path, char_t *query)
  {
  	char pptp_para[512];
	char *p;




	memset(pptp_para,0,sizeof(pptp_para));
	
	int run_sign=0;
	
	
		p=websGetVar(wp,T("pptp_active") , T(""));
			
             if(strstr(p,"on"))  {
                            run_sign=1;
		}else{
                             run_sign=0;
		}
		nvram_bufset(RT2860_NVRAM, "pptp_active",p);
		sprintf(pptp_para,"%s,",p);

              p=websGetVar(wp,T("pptp_user"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_user",p);
		strcat(pptp_para,p);strcat(pptp_para,",");

              p=websGetVar(wp,T("pptp_password"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_password",p);
		strcat(pptp_para,p);strcat(pptp_para,",");
		
		
		p=websGetVar(wp,T("pptp_server"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_server",p);
		strcat(pptp_para,p);strcat(pptp_para,",");	

		p=websGetVar(wp,T("pptp_remote_lan"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_remote_lan",p);
		strcat(pptp_para,p);strcat(pptp_para,"/");	

		p=websGetVar(wp,T("pptp_remote_mask"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_remote_mask",p);
		strcat(pptp_para,p);strcat(pptp_para,",");	

		p=websGetVar(wp,T("pptp_localip_mode"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_localip_mode",p);
		strcat(pptp_para,p);strcat(pptp_para,",");	

		p=websGetVar(wp,T("pptp_local_ip"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_local_ip",p);
		strcat(pptp_para,p);strcat(pptp_para,",");	
		
		p=websGetVar(wp,T("pptp_mppe_encryption"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_mppe_encryption",p);
		strcat(pptp_para,p);strcat(pptp_para,",");	

		p=websGetVar(wp,T("pptp_mppe_40"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_mppe_40",p);
		strcat(pptp_para,p);strcat(pptp_para,",");	
		
		p=websGetVar(wp,T("pptp_mppe_refuse_stateless"), T(""));
		nvram_bufset(RT2860_NVRAM, "pptp_mppe_refuse_stateless",p);
		strcat(pptp_para,p);

	nvram_set(RT2860_NVRAM,nvram_pptp_para, pptp_para);
	nvram_commit(RT2860_NVRAM);


	websRedirect(wp,"html/pptp.asp");

	pptp_run(run_sign);

	


	return;

		
}

			
void formDefinepptp(void)
{
	websFormDefine(T("pptp_action"), pptp_action);
	
}


void init_pptp(void)
{
	formDefinepptp();
//	load_pptp_para_to_buf();			  
}

