#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>


#define true 0
#define false -1

#define GET_MAC_CMD "iwpriv ra0 e2p "
#define VENDOR_ID "000C43"
#define HARDWARE_VERSION_STR "hardware_version"


//char  e2p_addr[]="0406082e3032";
//int get_count=6;

char  e2p_addr[]="2e3032";
int get_count=3;

char *VERSION[4]={
	"0.0.0", //other 
	"1.0.0", //base version
	"2.0.0", //professional version
	"3.0.0" //utimate version
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

	if(*ret_len>=4){
		strncpy(ret_buf,p+5,2);
		*(p+5)=0;
		strcat(ret_buf,p+3);
	
	}

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


int get_hardware_index(char*mac,int*index)
{
	char buf[20];
	unsigned char seg;
	memset(buf,0,sizeof(buf));
	strncpy(buf,mac,6);
	if(strcmp(buf,VENDOR_ID)){
		*index=0;
		return 1;
	}
	memset(buf,0,sizeof(buf));
	strncpy(buf,mac+6,2);
	seg=(unsigned char)(buf[0]*16+buf[1]);
	printf("seg=0X%02X\n",seg);
	if(seg>=0xaa){
		*index=3;
	}else if(seg>=0x55){
		*index=2;
	}else{
		*index=1;
	}
	return 1;
}

int set_hardware_version(char*version)
{
	char buf[50];
	strcpy(buf,"nvram_set ");
	strcat(buf,HARDWARE_VERSION_STR);
	strcat(buf," ");
	strcat(buf,version);
	system(buf);	
	return 1;
}

int main(int argc,char*argv[])
{

	char Mac_Str[30];
	char ret_tmp[30];
	char cmd_buf[20];
	int i;
	int len=0;
	int index;
	

	memset(Mac_Str,0,sizeof(Mac_Str));
	for(i=0;i<get_count;i++){	
		strcpy(cmd_buf,GET_MAC_CMD);
		strncat(cmd_buf,(e2p_addr+i*2),2);
		memset(ret_tmp,0,sizeof(ret_tmp));
		if(get_mac_addr_short(cmd_buf,ret_tmp,&len)==true){
			filter_cr_lf(ret_tmp,&len);
printf("get_mac_addr_short:%s  len:%d\n",ret_tmp,len);		
			strncat(Mac_Str,ret_tmp,len);
		}
	}


	if(strlen(Mac_Str)>0){
printf("MAC:%s\n",Mac_Str);
		get_hardware_index(Mac_Str,&index);
printf("index:%d\n",index);
		set_hardware_version(VERSION[index]);
		//printf("hard ware version=%s\n",&VERSION[index]);
	}else{
		printf("check device error!");
	}
	return 0;

}
