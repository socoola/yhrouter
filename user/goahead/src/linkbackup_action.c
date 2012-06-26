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

#include "linkbackup_action.h"

#define nvram_linkbackup_para "linkbackup_para"


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


     int load_linkbackup_para_to_buf(void)
   {
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_linkbackup_para);
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




   void link_backup_run(int sign)
  {
	if(sign==1){
		doSystem("start_linkbackup > /dev/console&");
	}else{
		doSystem("killall link_backup");
	}
  }




   static void linkbackup_action(webs_t wp, char_t *path, char_t *query)
  {
  	char linkbackup_para[1024];
	char *link_backup_status;
	char*link_backup_backtomain;
	char link_backup_first[10];
	char link_backup_second[10];
	char *link_backup_checkcount;


	char*linkbackup_cellural_priority_sel;
	char*linkbackup_wan_priority_sel;
	char*wan_port;

	
	char *link_backup_ping;
	char *link_backup_wan2_ping;
	
	char* link_backup_host;
	char* link_backup_wan2_host;



	
	int run_sign=0;
	
	
		link_backup_status=websGetVar(wp,T("link_backup_status") , T(""));
			
             if(strstr(link_backup_status,"on"))  {
			 
                            run_sign=1;
		}else{
                             run_sign=0;
		}
		nvram_bufset(RT2860_NVRAM, "link_backup_status",link_backup_status);


              link_backup_backtomain=websGetVar(wp,T("link_backup_backtomain"), T(""));
		nvram_bufset(RT2860_NVRAM, "link_backup_backtomain",link_backup_backtomain);


		linkbackup_cellural_priority_sel= websGetVar(wp, T("linkbackup_cellural_priority_sel"), T(""));
		if(strstr(linkbackup_cellural_priority_sel,"0"))  strcpy(link_backup_first,"3G");  
		else  strcpy(link_backup_second,"3G");
		
		linkbackup_wan_priority_sel= websGetVar(wp, T("linkbackup_wan_priority_sel"), T(""));
		wan_port= websGetVar(wp, T("linkbackup_wan_sel"), T(""));
		if(strstr(linkbackup_wan_priority_sel,"0"))  strcpy(link_backup_first,wan_port);  
		else  strcpy(link_backup_second,wan_port);
		
		nvram_bufset(RT2860_NVRAM, "link_backup_first",link_backup_first);
		nvram_bufset(RT2860_NVRAM, "link_backup_second",link_backup_second);

		link_backup_checkcount= websGetVar(wp, T("link_backup_checkcount"), T(""));											
		nvram_bufset(RT2860_NVRAM, "link_backup_checkcount",link_backup_checkcount);




		link_backup_ping= websGetVar(wp, T("wan1_ping_sel"), T(""));	
		nvram_bufset(RT2860_NVRAM, "link_backup_ping",link_backup_ping);
		
		link_backup_host= websGetVar(wp, T("link_backup_wan1_host"), T(""));	
		nvram_bufset(RT2860_NVRAM, "link_backup_host",link_backup_host);


		link_backup_wan2_ping= websGetVar(wp, T("wan2_ping_sel"), T(""));	
		nvram_bufset(RT2860_NVRAM, "link_backup_wan2_ping",link_backup_wan2_ping);
		
		link_backup_wan2_host= websGetVar(wp, T("link_backup_wan2_host"), T(""));	
		nvram_bufset(RT2860_NVRAM, "link_backup_wan2_host",link_backup_wan2_host);



			memset(linkbackup_para,0,sizeof(linkbackup_para));
			sprintf(linkbackup_para,"%s,%s,%s,%s,%s,%s,%s,%s,%s",
					link_backup_status,link_backup_backtomain,link_backup_first,link_backup_second, link_backup_checkcount,link_backup_ping,
                                   link_backup_host,link_backup_wan2_ping,link_backup_wan2_host
			);







	//printf("rulesbuf:%s\n", rulesbuf);
	nvram_set(RT2860_NVRAM,nvram_linkbackup_para, linkbackup_para);
	nvram_commit(RT2860_NVRAM);


//	load_fail_over_para_to_buf();

	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "internet/linkbackup.asp");



//ipsec_run(); 
	link_backup_run(run_sign);



	return;

		
}

			
void formDefinelinkbackup(void)
{
	websFormDefine(T("linkbackup_action"), linkbackup_action);
	
}


void init_linkbackup(void)
{
	formDefinelinkbackup();
	load_linkbackup_para_to_buf();			  
}

