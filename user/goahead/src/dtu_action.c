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

#include "dtu_action.h"

#define nvram_dtu_para "dtu_para"


#define true 1
#define false 0



int locate_para(char*para,int num,char ch,char* ret)
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


int setbuf_one_para(char* para)
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
	nvram_bufset(RT2860_NVRAM, "dtu_status",str_tmp);
	//fprintf(fp,"dtu_status:%s\n",str_tmp);


//serial 
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,1,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "dtu_bandrate",str_tmp);
	//fprintf(fp,"dtu_bandrate:%s\n",str_tmp);

	

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,2,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "dtu_databits",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,3,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "dtu_parity",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,4,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "dtu_stopbits",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,5,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "dtu_flowcontrol",str_tmp);
	//fprintf(fp,"dtu_flowcontrol:%s\n",str_tmp);

//serial  end



//tcp udp
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,6,',',str_tmp))==false){	

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "dtu_socktype",str_tmp);
	//fprintf(fp,"dtu_socktype:%s\n",str_tmp);


// server client
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,7,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "dtu_linktype",str_tmp);
	//fprintf(fp,"dtu_linktype:%s\n",str_tmp);



	
		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,8,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_serverport",str_tmp);
		//fprintf(fp,"dtu_serverport:%s\n",str_tmp);



		//center 1
		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,9,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center1_status",str_tmp);
		//fprintf(fp,"dtu_center1_status:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,10,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center1_ip",str_tmp);
		//fprintf(fp,"dtu_center1_ip:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,11,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center1_port",str_tmp);
		//fprintf(fp,"dtu_center1_port:%s\n",str_tmp);



		//center 2
		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,12,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center2_status",str_tmp);
		//fprintf(fp,"dtu_center2_status:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,13,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center2_ip",str_tmp);
		//fprintf(fp,"dtu_center2_ip:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,14,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center2_port",str_tmp);
		//fprintf(fp,"dtu_center2_port:%s\n",str_tmp);

		//center 3
		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,15,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center3_status",str_tmp);
		//fprintf(fp,"dtu_center3_status:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,16,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center3_ip",str_tmp);
		//fprintf(fp,"dtu_center3_ip:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,17,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center3_port",str_tmp);
		//fprintf(fp,"dtu_center3_port:%s\n",str_tmp);

		//center 4
		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,18,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center4_status",str_tmp);
		//fprintf(fp,"dtu_center4_status:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,19,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center4_ip",str_tmp);
		//fprintf(fp,"dtu_center4_ip:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,20,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_center4_port",str_tmp);
		//fprintf(fp,"dtu_center4_port:%s\n",str_tmp);



		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,21,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_h_time",str_tmp);
		//fprintf(fp,"dtu_h_time:%s\n",str_tmp);
		
		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,22,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_h_info_type",str_tmp);

		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,23,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_h_info",str_tmp);
	//	fprintf(fp,"dtu_h_info:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,24,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_send_data_time",str_tmp);

		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,25,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_off_heart_after_nodata_recv",str_tmp);
		
		memset(str_tmp,0,sizeof(str_tmp));	
		if((ret=locate_para(para,26,',',str_tmp))==false){

			return false;
		}
		nvram_bufset(RT2860_NVRAM, "dtu_off_heart_after_nodata_time",str_tmp);

	//fseek(fp,0,SEEK_SET);
	//fprintf(fp,"make config file succeed!",str_tmp);
	//fclose(fp);


	return true;

	
}



int load_dtu_para_to_buf(void)
{
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_dtu_para);
	if(para==NULL){
		printf("read dtu_para false!");
		return false;
	}
	if(setbuf_one_para(para)==false){
		printf(" dtu init para false!\n");
		return false;
	}
	printf(" dtu init para succeed!\n");
	return true;
}



void dtu_run(int sign)
{
	if(sign==1){
		doSystem("killall dtu");
		doSystem("start_dtu&");
	}else{
		doSystem("killall dtu");

	}
}








static void dtu_action(webs_t wp, char_t *path, char_t *query)
{
	char dtu_para[1024];
	char *dtu_status;
	char *dtu_bandrate,*dtu_databits,*dtu_parity,*dtu_stopbits,*dtu_flowcontrol;
	char *dtu_socktype,*dtu_linktype;
	char *dtu_serverport;
	char *dtu_center1_status,*dtu_center1_ip,*dtu_center1_port;
	char *dtu_center2_status,*dtu_center2_ip,*dtu_center2_port;
	char *dtu_center3_status,*dtu_center3_ip,*dtu_center3_port;
	char *dtu_center4_status,*dtu_center4_ip,*dtu_center4_port;
	char *dtu_h_time,*dtu_h_info_type,*dtu_h_info,*dtu_send_data_time;
	char *dtu_off_heart_after_nodata_recv,*dtu_off_heart_after_nodata_time;

	char dtu_off_heart_after_nodata_recv_tmp[6];
	
	char dtu_center1_status_tmp[6],dtu_center2_status_tmp[6],dtu_center3_status_tmp[6],dtu_center4_status_tmp[6];
	char dtu_h_info_type_tmp[5];

	
	
	int run_sign=0;
	
	
	 	dtu_status = websGetVar(wp, T("dtu_status_sel"), T(""));
		if(strstr(dtu_status,"on")) run_sign=1;
		else run_sign=0;
		nvram_bufset(RT2860_NVRAM, "dtu_status",dtu_status);
		dtu_bandrate = websGetVar(wp, T("dtu_bandrate_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "dtu_bandrate",dtu_bandrate);
		dtu_databits = websGetVar(wp, T("dtu_databits_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "dtu_databits",dtu_databits);
		dtu_parity = websGetVar(wp, T("dtu_parity_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "dtu_parity",dtu_parity);
		dtu_stopbits = websGetVar(wp, T("dtu_stopbits_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "dtu_stopbits",dtu_stopbits);
		dtu_flowcontrol = websGetVar(wp, T("dtu_flowcontrol_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "dtu_flowcontrol",dtu_flowcontrol);
		dtu_socktype = websGetVar(wp, T("dtu_socktype_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "dtu_socktype",dtu_socktype);
		dtu_linktype = websGetVar(wp, T("dtu_linktype_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "dtu_linktype",dtu_linktype);
		

			dtu_serverport = websGetVar(wp, T("dtu_serverport"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_serverport",dtu_serverport);
			
			

			memset(dtu_center1_status_tmp,0,sizeof(dtu_center1_status_tmp));
			dtu_center1_status = websGetVar(wp, T("center_1_check"), T(""));
			if(strstr(dtu_center1_status,"on"))  strcpy(dtu_center1_status_tmp,"en"); 
			else  strcpy(dtu_center1_status_tmp,"dis"); 
			nvram_bufset(RT2860_NVRAM, "dtu_center1_status",dtu_center1_status_tmp);	
			dtu_center1_ip = websGetVar(wp, T("dtu_center_1_ip"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_center1_ip",dtu_center1_ip);
			dtu_center1_port = websGetVar(wp, T("dtu_center_1_port"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_center1_port",dtu_center1_port);

			memset(dtu_center2_status_tmp,0,sizeof(dtu_center2_status_tmp));
			dtu_center2_status = websGetVar(wp, T("center_2_check"), T(""));
			if(strstr(dtu_center2_status,"on"))  strcpy(dtu_center2_status_tmp,"en"); 
			else  strcpy(dtu_center2_status_tmp,"dis"); 
			nvram_bufset(RT2860_NVRAM, "dtu_center2_status",dtu_center2_status_tmp);	
			dtu_center2_ip = websGetVar(wp, T("dtu_center_2_ip"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_center2_ip",dtu_center2_ip);
			dtu_center2_port = websGetVar(wp, T("dtu_center_2_port"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_center2_port",dtu_center2_port);

			memset(dtu_center3_status_tmp,0,sizeof(dtu_center3_status_tmp));
			dtu_center3_status = websGetVar(wp, T("center_3_check"), T(""));
			if(strstr(dtu_center3_status,"on"))  strcpy(dtu_center3_status_tmp,"en"); 
			else  strcpy(dtu_center3_status_tmp,"dis"); 
			nvram_bufset(RT2860_NVRAM, "dtu_center3_status",dtu_center3_status_tmp);	
			dtu_center3_ip = websGetVar(wp, T("dtu_center_3_ip"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_center3_ip",dtu_center3_ip);
			dtu_center3_port = websGetVar(wp, T("dtu_center_3_port"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_center3_port",dtu_center3_port);
			
			memset(dtu_center4_status_tmp,0,sizeof(dtu_center4_status_tmp));
			dtu_center4_status = websGetVar(wp, T("center_4_check"), T(""));
			if(strstr(dtu_center4_status,"on"))  strcpy(dtu_center4_status_tmp,"en"); 
			else  strcpy(dtu_center4_status_tmp,"dis"); 
			nvram_bufset(RT2860_NVRAM, "dtu_center4_status",dtu_center4_status_tmp);		
			dtu_center4_ip = websGetVar(wp, T("dtu_center_4_ip"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_center4_ip",dtu_center4_ip);
			dtu_center4_port = websGetVar(wp, T("dtu_center_4_port"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_center4_port",dtu_center4_port);

			dtu_h_time = websGetVar(wp, T("dtu_h_time"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_h_time",dtu_h_time);
			
			memset(dtu_h_info_type_tmp,0,sizeof(dtu_h_info_type_tmp));
			dtu_h_info_type = websGetVar(wp, T("dtu_h_info_type"), T(""));
			if(strstr(dtu_h_info_type,"on"))  strcpy(dtu_h_info_type_tmp,"hex"); 
			else  strcpy(dtu_h_info_type_tmp,"str"); 
			nvram_bufset(RT2860_NVRAM, "dtu_h_info_type",dtu_h_info_type_tmp);
			
			dtu_h_info = websGetVar(wp, T("dtu_h_info"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_h_info",dtu_h_info);

			dtu_send_data_time = websGetVar(wp, T("dtu_send_data_time"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_send_data_time",dtu_send_data_time);




			memset(dtu_off_heart_after_nodata_recv_tmp,0,sizeof(dtu_off_heart_after_nodata_recv_tmp));
			dtu_off_heart_after_nodata_recv = websGetVar(wp, T("dtu_off_heart_after_nodata_recv"), T(""));
			if(strstr(dtu_off_heart_after_nodata_recv,"on"))  strcpy(dtu_off_heart_after_nodata_recv_tmp,"en"); 
			else  strcpy(dtu_off_heart_after_nodata_recv_tmp,"dis"); 
			nvram_bufset(RT2860_NVRAM, "dtu_off_heart_after_nodata_recv",dtu_off_heart_after_nodata_recv_tmp);	



			dtu_off_heart_after_nodata_time = websGetVar(wp, T("dtu_off_heart_after_nodata_time"), T(""));
			nvram_bufset(RT2860_NVRAM, "dtu_off_heart_after_nodata_time",dtu_off_heart_after_nodata_time);
			
			memset(dtu_para,0,sizeof(dtu_para));
			snprintf(dtu_para,sizeof(dtu_para),"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
					dtu_status,dtu_bandrate,dtu_databits,dtu_parity,dtu_stopbits,dtu_flowcontrol,dtu_socktype,dtu_linktype,dtu_serverport,
					dtu_center1_status_tmp,dtu_center1_ip,dtu_center1_port,
					dtu_center2_status_tmp,dtu_center2_ip,dtu_center2_port,
					dtu_center3_status_tmp,dtu_center3_ip,dtu_center3_port,
					dtu_center4_status_tmp,dtu_center4_ip,dtu_center4_port,
					dtu_h_time,dtu_h_info_type_tmp,dtu_h_info,dtu_send_data_time,
					dtu_off_heart_after_nodata_recv_tmp,dtu_off_heart_after_nodata_time
					);







	//printf("rulesbuf:%s\n", rulesbuf);
	nvram_set(RT2860_NVRAM, nvram_dtu_para, dtu_para);
	nvram_commit(RT2860_NVRAM);


//	load_dtu_para_to_buf();

	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "internet/dtu.asp");



//ipsec_run(); 
	dtu_run(run_sign);



	return;

		
}








void formDefineDTU(void)
{
	websFormDefine(T("dtu_action"), dtu_action);
	
}


void init_dtu(void)
{
	formDefineDTU();
	load_dtu_para_to_buf();
	
}


