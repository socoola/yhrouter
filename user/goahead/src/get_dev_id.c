#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


#define true 0
#define false -1

#define GET_MAC_CMD "iwpriv ra0 e2p "


//char  e2p_addr[]="0406082e3032";
//int get_count=6;

char  e2p_addr[]="2e300832";
int get_count=4;



#define PTR_NUL 16   //get_count*4
struct change_ptr{
	int in_p;
	int out_p;
}PTR[PTR_NUL]={
		{0,12},
		{1,9},
		{2,5},
		{3,10},
		
		{4,14},
		{5,2},
		{6,13},
		{7,4},
		
		{8,11},
		{9,1},
		{10,15},
		{11,0},
		
		{12,7},
		{13,3},
		{14,6},
		{15,8},
};



int get_mac_addr_short(char*cmd,char*ret_buf,int* ret_len)
{
	FILE*fp;
	char buf[30];
	char*p;
	if((cmd==NULL)||(ret_buf==NULL)||(ret_len==NULL)) return false;

	memset(buf,0,sizeof(buf));
	
	if((fp=popen(cmd,"r"))==NULL){
		return false;
	}
	if(fgets(buf,sizeof(buf),fp)==NULL) return false;
	memset(buf,0,sizeof(buf));
	if(fgets(buf,sizeof(buf),fp)==NULL) return false;
	if(fp) pclose(fp);

	if((p=strchr(buf,':'))==NULL) return false;

	*ret_len=strlen(p+3);



	strncpy(ret_buf,p+3,*ret_len);

	return true;

	
}

int filter_cr_lf(char*str,int*len)
{
	int i;
	if(str==NULL) return false;
	/*
	for(i=0;i<*len;i++){
		if((*(str+i)==0x0d)||(*(str+i)==0x0a)){
			*(str+i)=0;
		}
	}
	*len=strlen(str);
	*/

	*len=4;
	return true;
}


int handle_mac_str(char*in,int str_len,char*out)
{
	int i;

	for(i=0;i<PTR_NUL;i++){
		*(out+PTR[i].out_p)=*(in+PTR[i].in_p);
	}
	return true;
}




int main(int argc,char*argv[])
{

	char Mac_Str[30];
	char ret_tmp[30];
	char cmd_buf[20];
	int i;
	int len=0;
	

	memset(Mac_Str,0,sizeof(Mac_Str));
	for(i=0;i<get_count;i++){	
		strcpy(cmd_buf,GET_MAC_CMD);
		strncat(cmd_buf,(e2p_addr+i*2),2);
		memset(ret_tmp,0,sizeof(ret_tmp));
		if(get_mac_addr_short(cmd_buf,ret_tmp,&len)==true){
			filter_cr_lf(ret_tmp,&len);
//printf("get_mac_addr_short:%s  len:%d\n",ret_tmp,len);		
			strncat(Mac_Str,ret_tmp,len);
		}
	}


	memset(ret_tmp,0,sizeof(ret_tmp));
	if(strlen(Mac_Str)>0){
		//printf("Old_id:%s\n",Mac_Str);
		handle_mac_str(Mac_Str,strlen(Mac_Str),ret_tmp);
		printf("%s",ret_tmp);
	}else{
		printf("get device id error!");
	}
	return 0;

}
