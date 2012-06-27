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

#include "gps_action.h"

#define nvram_gps_para "gps_para"


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
	nvram_bufset(RT2860_NVRAM, "gps_status",str_tmp);
	//fprintf(fp,"gps_status:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,1,',',str_tmp))==false){
		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_sendto",str_tmp);
	//fprintf(fp,"gps_status:%s\n",str_tmp);

//serial 
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,2,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_serial_baudrate",str_tmp);
	//fprintf(fp,"gps_bandrate:%s\n",str_tmp);

	

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,3,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_serial_databits",str_tmp);
	//fprintf(fp,"gps_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,4,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_serial_parity",str_tmp);
	//fprintf(fp,"gps_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,5,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_serial_stopbits",str_tmp);
	//fprintf(fp,"gps_stopbits:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,6,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_serial_flowcontrol",str_tmp);
	//fprintf(fp,"gps_flowcontrol:%s\n",str_tmp);

//serial  end

//tcp udp
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,7,',',str_tmp))==false){	

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_server_socktype",str_tmp);
	//fprintf(fp,"gps_socktype:%s\n",str_tmp);


// server client
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,8,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_server_ip",str_tmp);
	//fprintf(fp,"gps_linktype:%s\n",str_tmp);



	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,9,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "gps_server_port",str_tmp);
		//fprintf(fp,"gps_serverport:%s\n",str_tmp);

	//fseek(fp,0,SEEK_SET);
	//fprintf(fp,"make config file succeed!",str_tmp);
	//fclose(fp);


	return true;

	
}



int load_gps_para_to_buf(void)
{
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_gps_para);
	if(para==NULL){
		printf("read gps_para false!");
		return false;
	}
	if(setbuf_one_para(para)==false){
		printf(" gps init para false!\n");
		return false;
	}
	printf(" gps init para succeed!\n");
	return true;
}



void gps_run(int sign)
{
	if(sign==1){
	//	doSystem("killall gps");
		doSystem("start_gps.sh&");
	}else{
		doSystem("killall gps");

	}
}








static void gps_action(webs_t wp, char_t *path, char_t *query)
{
	char gps_para[1024];
	char *gps_status,*gps_sendto;
	char *gps_baudrate,*gps_databits,*gps_parity,*gps_stopbits,*gps_flowcontrol;
	char *gps_socktype,*gps_server_ip,*gps_server_port;
	
	int run_sign=0;
	
	
	 	gps_status = websGetVar(wp, T("gps_atvice"), T(""));
		if(strstr(gps_status,"on")) run_sign=1;
		else run_sign=0;
		nvram_bufset(RT2860_NVRAM, "gps_status",gps_status);
		
		gps_sendto = websGetVar(wp, T("gps_sendto_inf"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_sendto",gps_sendto);
		
		gps_baudrate = websGetVar(wp, T("serial_baudrate_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_serial_baudrate",gps_baudrate);
		gps_databits = websGetVar(wp, T("serial_databits_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_serial_databits",gps_databits);
		gps_parity = websGetVar(wp, T("serial_parity_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_serial_parity",gps_parity);
		gps_stopbits = websGetVar(wp, T("serial_stopbits_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_serial_stopbits",gps_stopbits);
		gps_flowcontrol = websGetVar(wp, T("serial_flowcontrol_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_serial_flowcontrol",gps_flowcontrol);
		gps_socktype = websGetVar(wp, T("gps_socktype_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_server_socktype",gps_socktype);
		gps_server_ip = websGetVar(wp, T("gps_server_ip"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_server_ip",gps_server_ip);
		gps_server_port = websGetVar(wp, T("gps_server_port"), T(""));
		nvram_bufset(RT2860_NVRAM, "gps_server_port",gps_server_port);
			
			
		memset(gps_para,0,sizeof(gps_para));
		snprintf(gps_para,sizeof(gps_para),"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
					gps_status,gps_sendto,gps_baudrate,gps_databits,gps_parity,gps_stopbits,gps_flowcontrol,gps_socktype,gps_server_ip,gps_server_port
					);


	nvram_set(RT2860_NVRAM, nvram_gps_para, gps_para);
	nvram_commit(RT2860_NVRAM);


//	load_gps_para_to_buf();

	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "internet/gps.asp");



//ipsec_run(); 
	gps_run(run_sign);



	return;

		
}








void formDefineGPS(void)
{
	websFormDefine(T("gps_action"), gps_action);
	
}


void init_gps(void)
{
	formDefineGPS();
	load_gps_para_to_buf();
	
}


