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

#include "msg_action.h"

#define nvram_msg_para "msg_para"


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
	nvram_bufset(RT2860_NVRAM, "msg_status",str_tmp);
	//fprintf(fp,"dtu_status:%s\n",str_tmp);



	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,1,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_send_ask",str_tmp);
	//fprintf(fp,"dtu_bandrate:%s\n",str_tmp);

	

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,2,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num1_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,3,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num1_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,4,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num1_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);





		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,5,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num2_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,6,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num2_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,7,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num2_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);


		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,8,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num3_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,9,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num3_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,10,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num3_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);



		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,11,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num4_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,12,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num4_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,13,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num4_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);

		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,14,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num5_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,15,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num5_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,16,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num5_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);

		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,17,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num6_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,18,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num6_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,19,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num6_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);

		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,20,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num7_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,21,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num7_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,22,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num7_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);

		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,23,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num8_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,24,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num8_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,25,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num8_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);

		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,26,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num9_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,27,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num9_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,28,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num9_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);

		memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,29,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num10_num",str_tmp);
	//fprintf(fp,"dtu_databits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,30,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num10_msg",str_tmp);
	//fprintf(fp,"dtu_parity:%s\n",str_tmp);

	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,31,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_num10_voi",str_tmp);
	//fprintf(fp,"dtu_stopbits:%s\n",str_tmp);


	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,32,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_cmd1",str_tmp);



	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,33,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_cmd2",str_tmp);

	
	memset(str_tmp,0,sizeof(str_tmp));	
	if((ret=locate_para(para,34,',',str_tmp))==false){

		return false;
	}
	nvram_bufset(RT2860_NVRAM, "msg_voice_cmd",str_tmp);


	return true;

	
}



int load_msg_para_to_buf(void)
{
	char *para;

	para = nvram_bufget(RT2860_NVRAM, nvram_msg_para);
	if(para==NULL){
		printf("read msg_para false!");
		return false;
	}
	if(setbuf_one_para(para)==false){
		printf(" msg init para false!\n");
		return false;
	}
	printf(" msg init para succeed!\n");
	return true;
}



void msg_run(int sign)
{
	if(sign==1){
		doSystem("killall msg_cmd");
		doSystem("start_msg_cmd&");
	}else{
		doSystem("killall msg_cmd");
	}
}








static void msg_action(webs_t wp, char_t *path, char_t *query)
{
	int run_sign=0;
	char msg_para[1024];
	char *msg_status;
	char *msg_send_ask;
	char *msg_num1_num,*msg_num1_msg_check,*msg_num1_voi_check;
	char *msg_num2_num,*msg_num2_msg_check,*msg_num2_voi_check;
	char *msg_num3_num,*msg_num3_msg_check,*msg_num3_voi_check;	
	char *msg_num4_num,*msg_num4_msg_check,*msg_num4_voi_check;
	char *msg_num5_num,*msg_num5_msg_check,*msg_num5_voi_check;
	char *msg_num6_num,*msg_num6_msg_check,*msg_num6_voi_check;
	char *msg_num7_num,*msg_num7_msg_check,*msg_num7_voi_check;
	char *msg_num8_num,*msg_num8_msg_check,*msg_num8_voi_check;
	char *msg_num9_num,*msg_num9_msg_check,*msg_num9_voi_check;
	char *msg_num10_num,*msg_num10_msg_check,*msg_num10_voi_check;
	char *msg_cmd_1,*msg_cmd_2;

	char *msg_voice_cmd;


	
	 	msg_status = websGetVar(wp, T("msg_status_sel"), T(""));
		if(strstr(msg_status,"on"))	run_sign=1;
		else 	run_sign=0;
		nvram_bufset(RT2860_NVRAM, "msg_status",msg_status);

		
		msg_send_ask = websGetVar(wp, T("msg_send_ask_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_send_ask",msg_send_ask);
		
		msg_num1_num = websGetVar(wp, T("msg_num1_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num1_num",msg_num1_num);
		
		msg_num1_msg_check = websGetVar(wp, T("msg_num1_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num1_msg",msg_num1_msg_check);
		
		msg_num1_voi_check = websGetVar(wp, T("msg_num1_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num1_voi",msg_num1_voi_check);


		msg_num2_num = websGetVar(wp, T("msg_num2_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num2_num",msg_num2_num);
		
		msg_num2_msg_check = websGetVar(wp, T("msg_num2_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num2_msg",msg_num2_msg_check);
		
		msg_num2_voi_check = websGetVar(wp, T("msg_num2_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num2_voi",msg_num2_voi_check);

				msg_num3_num = websGetVar(wp, T("msg_num3_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num3_num",msg_num3_num);
		
		msg_num3_msg_check = websGetVar(wp, T("msg_num3_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num3_msg",msg_num3_msg_check);
		
		msg_num3_voi_check = websGetVar(wp, T("msg_num3_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num3_voi",msg_num3_voi_check);

				msg_num4_num = websGetVar(wp, T("msg_num4_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num4_num",msg_num4_num);
		
		msg_num4_msg_check = websGetVar(wp, T("msg_num4_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num4_msg",msg_num4_msg_check);
		
		msg_num4_voi_check = websGetVar(wp, T("msg_num4_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num4_voi",msg_num4_voi_check);

				msg_num5_num = websGetVar(wp, T("msg_num5_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num5_num",msg_num5_num);
		
		msg_num5_msg_check = websGetVar(wp, T("msg_num5_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num5_msg",msg_num5_msg_check);
		
		msg_num5_voi_check = websGetVar(wp, T("msg_num5_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num5_voi",msg_num5_voi_check);

				msg_num6_num = websGetVar(wp, T("msg_num6_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num6_num",msg_num6_num);
		
		msg_num6_msg_check = websGetVar(wp, T("msg_num6_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num6_msg",msg_num6_msg_check);
		
		msg_num6_voi_check = websGetVar(wp, T("msg_num6_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num6_voi",msg_num6_voi_check);

				msg_num7_num = websGetVar(wp, T("msg_num7_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num7_num",msg_num7_num);
		
		msg_num7_msg_check = websGetVar(wp, T("msg_num7_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num7_msg",msg_num7_msg_check);
		
		msg_num7_voi_check = websGetVar(wp, T("msg_num7_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num7_voi",msg_num7_voi_check);

				msg_num8_num = websGetVar(wp, T("msg_num8_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num8_num",msg_num8_num);
		
		msg_num8_msg_check = websGetVar(wp, T("msg_num8_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num8_msg",msg_num8_msg_check);
		
		msg_num8_voi_check = websGetVar(wp, T("msg_num8_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num8_voi",msg_num8_voi_check);

				msg_num9_num = websGetVar(wp, T("msg_num9_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num9_num",msg_num9_num);
		
		msg_num9_msg_check = websGetVar(wp, T("msg_num9_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num9_msg",msg_num9_msg_check);
		
		msg_num9_voi_check = websGetVar(wp, T("msg_num9_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num9_voi",msg_num9_voi_check);

				msg_num10_num = websGetVar(wp, T("msg_num10_num"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num10_num",msg_num10_num);
		
		msg_num10_msg_check = websGetVar(wp, T("msg_num10_msg_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num10_msg",msg_num10_msg_check);
		
		msg_num10_voi_check = websGetVar(wp, T("msg_num10_voi_check"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_num10_voi",msg_num10_voi_check);

		msg_cmd_1 = websGetVar(wp, T("msg_cmd_1_str"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_cmd1",msg_cmd_1);
		msg_cmd_2 = websGetVar(wp, T("msg_cmd_2_str"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_cmd2",msg_cmd_2);
		


		
		msg_voice_cmd = websGetVar(wp, T("msg_voice_cmd_sel"), T(""));
		nvram_bufset(RT2860_NVRAM, "msg_voice_cmd",msg_voice_cmd);
		




			memset(msg_para,0,sizeof(msg_para));
			snprintf(msg_para,sizeof(msg_para),"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",
					msg_status,
					msg_send_ask,
					msg_num1_num,msg_num1_msg_check,msg_num1_voi_check,
					msg_num2_num,msg_num2_msg_check,msg_num2_voi_check,
					msg_num3_num,msg_num3_msg_check,msg_num3_voi_check,	
					msg_num4_num,msg_num4_msg_check,msg_num4_voi_check,
					msg_num5_num,msg_num5_msg_check,msg_num5_voi_check,
					msg_num6_num,msg_num6_msg_check,msg_num6_voi_check,
					msg_num7_num,msg_num7_msg_check,msg_num7_voi_check,
					msg_num8_num,msg_num8_msg_check,msg_num8_voi_check,
					msg_num9_num,msg_num9_msg_check,msg_num9_voi_check,
					msg_num10_num,msg_num10_msg_check,msg_num10_voi_check,
					msg_cmd_1,msg_cmd_2,
					msg_voice_cmd
					);







	//printf("rulesbuf:%s\n", rulesbuf);
	nvram_set(RT2860_NVRAM, nvram_msg_para, msg_para);
	nvram_commit(RT2860_NVRAM);


//	load_dtu_para_to_buf();

	/* 完成转到ipsec 配置页面*/
	websRedirect(wp, "internet/msg.asp");



//ipsec_run(); 
	msg_run(run_sign);



	return;

		
}








void formDefineMSG(void)
{
	websFormDefine(T("msg_action"), msg_action);
}


void init_msg(void)
{
	formDefineMSG();
	load_msg_para_to_buf();
	
}


