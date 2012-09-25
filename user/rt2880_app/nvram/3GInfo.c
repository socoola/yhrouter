#include<stdio.h>
#include<unistd.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <time.h>
#include <termios.h>
#include "serial.h"



#define false 0
#define true 1


#define TIMEOUT_STR	"TimeOut\n"
#define TIMEOUT_TIME	1	


#define WAIT_OK 	0
#define WAIT_VALUE 	1
#define WAIT_NONE 	2
#define WAIT_SEND_MSG 	3

//#define DEBUG_DISPLAY_READ




#define default_device "/dev/ttyUSB0"
#define default_command "AT+CSQ"
#define default_sign ""
#define default_max_signal "0-31,100-199"






char para_in[4][30];
int fd=0;



void clear_fd(int* fd)
{
	*fd=0;
}

void timeout_handle(int signun)
{
	if(fd) close(fd);
	clear_fd(&fd);
        printf("%s",TIMEOUT_STR);
        exit(EXIT_SUCCESS);
}



















int open_device(int* fd,char* device)
{
        int fd_temp;
	if(fd==NULL) return false;
	
	//printf("open device");


        if((fd_temp=open(device,O_RDWR))<0)//open device
        {
                return false;
        }
	*fd=fd_temp;

        return true;
}

void close_device(int fd)
{
        if(fd)
                close(fd);
	
}
/*
int send_read(int fd,char*cmd,char* read_str,int wait_sign)
{
	char buf_echo[30];
	char*p_temp;
	char buf_temp[100];
	int n=0;
	char*p1,*p2;	
        char*p;
        int i;
        static char buf[1000];
        int readsize;
        int ret=false;
        if(read_str==NULL) return ret;
	p1=buf;
	strcpy(buf_echo,"echo \"at\" > ");
	strcat(buf_echo,para_in[0]);

      p=cmd;
        p+=3;
        i=0;

	if((p_temp=strchr(p,'='))||(p_temp=strchr(p,'?'))||(p_temp=strchr(p,0x0d)))	//0x0d= \r
	{
		memset(buf_temp,0,sizeof(buf_temp));
		n=p_temp-p;
		strncpy(buf_temp,p,n);
		//if(!strcmp(buf_temp,"GSN")) strcpy(buf_temp,"GSN");
	}


#ifdef DEBUG_DISPLAY_READ
        printf("write:%s\n",cmd);
#endif
	system(buf_echo);
        write(fd,cmd,(strlen(cmd)));
        while(1)
        {
                memset(buf,0,sizeof(buf));
               //buf[0]=0;
                readsize=read(fd,buf,sizeof(buf));
                buf[readsize]=0;
#ifdef DEBUG_DISPLAY_READ
        printf("read_all buf:%s  read size:%d\n",buf,readsize);
#endif
                if(wait_sign==WAIT_VALUE)
                {
                     	//for(i=0;i<10;i++)
                     	i=0;
                     	while(i<10)
                     	{
				//if(buf[0]=='A') break;
				if((buf[i]!=0x0d)&&(buf[i]!=0x0a))
				{
					p1=&buf[i];
					break;	
				}
				i++;
			}
			if((p1!=NULL)&&((*p1!='^')||(*p1!='+')||(*p1!='%')||(*p1!='*')))
			{
				if((*p1=='A')&&(*(p1+1)=='T')&&(*(p1+2)=='^'))
				{
					p1=strchr(p1+3,'^');
					if(p1==NULL) p1=buf;
				}
				else if((*p1=='A')&&(*(p1+1)=='T')&&(*(p1+2)=='+'))
				{
					p1=strchr(p1+3,'+');
					if(p1==NULL) p1=buf;
				}
				else if((*p1=='A')&&(*(p1+1)=='T')&&(*(p1+2)=='%'))
				{
					p1=strchr(p1+3,'%');
					if(p1==NULL) p1=buf;
				}
				else if((*p1=='A')&&(*(p1+1)=='T')&&(*(p1+2)=='*'))
				{
					p1=strchr(p1+3,'*');
					p1=strchr(p1+1,'*');
					if(p1==NULL) p1=buf;
				}
			}
                        //if(((*p1=='^')||(*p1=='+'))&&(*p++==*(p1+1))&&(*p==*(p1+2)))
    //   printf("send read:p1:%s,buf_temp:%s\n",p1,buf_temp);
#if defined(MODULE_MU103)
			if(strstr(buf_temp,"CGMR")||strstr(buf_temp,"CGSN"))
			{
				if((strlen(p1)>15)&&(*p1>='0')&&(*p1<='9'))  
				{
                                	#ifdef DEBUG_DISPLAY_READ
	                                 printf("read buf p1:%s\n",p1);
        	                        #endif
					strcpy(buf,p1);
					p2=buf;	
					while(*p2!=0) 
					{
						if((*p2==0x0d)||(*p2==0x0a)) *p2=0;	
						p2++;
					} 
	                                #ifdef DEBUG_DISPLAY_READ
        	                         printf("handled buf:%s\n",buf);
                	                #endif
                        	     ret=true;
                                	break;
						
				}		
			}
#endif
                        if(((*p1=='^')||(*p1=='+')||(*p1=='%')||(*p1=='*'))&&(strstr(p1,buf_temp)))
                        {
				
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("read buf:%s\n",buf);
                                #endif
				p2=p1;	
				while(*p2!=0) 
				{
					if((*p2==0x0d)||(*p2==0x0a)) *p2=0;	
					p2++;
				} 
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("handled buf:%s\n",buf);
                                #endif
                             ret=true;
                                break;
                         }

                        if(strstr(cmd,"AT+CMGL")||strstr(cmd,"AT^HCMGL")) //not dusimr command
                        {
				if(strstr(buf,"OK"))
				{
                                        #ifdef DEBUG_DISPLAY_READ
                                         printf("read buf:%s\n",buf);
                                        #endif
                                        ret=true;
                                        break;
	
				}
                                if(strstr(buf,"+CMS ERROR") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER"))
                                {
                                        #ifdef DEBUG_DISPLAY_READ
                                         printf("read buf:%s\n",buf);
                                        #endif
                                        ret=false;
                                        break;
                                }
                        }




                                if(strstr(buf,"+CMS ERROR") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER"))
                                {
                                        #ifdef DEBUG_DISPLAY_READ
                                         printf("read buf:%s\n",buf);
                                        #endif
                                        ret=false;
                                        break;
                                }

			if(strstr(buf,"ERROR")){

				ret=false;
				break;
			}
                }
                else if(wait_sign==WAIT_OK)
                {
			if(strstr(buf,"OK"))
                	{
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("read buf:%s\n",buf);
                                #endif
                        	ret=true;
				break;
                	}
			if(strstr(buf,"+CMS ERROR") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER"))
                	{
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("read buf:%s\n",buf);
                                #endif
                        	ret=false;
				break;
                	}



                }
		else if(wait_sign==WAIT_NONE) //for issue at command
		{
		
	               if((buf[0]!='\n')&& !strstr(buf,"^HRSSILVL") &&  !strstr(buf,"^RSSILVL"))
                        {
                                printf("%s",buf);
                                printf("\n");
                        }
                        if(strstr(buf,"OK") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER"))
			{
				ret=true;
				 break;
			}

		}
		else if(wait_sign==WAIT_SEND_MSG)	
		{
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("read buf:%s\n",buf);
                                #endif
			if(strstr(buf,">"))
			{
				ret=true;
				break;
			}
                        if(strstr(buf,"+CMS ERROR") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER"))
                        {
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("read buf:%s\n",buf);
                                #endif
                                ret=false;
                                break;
                        }

		}
             if(i++>15) break;
         }

        if(ret==true)
        {
                strcpy(read_str,buf);
        }
        else
        {
                strcpy(read_str,"");
        }
        return ret;
}


*/


void delay(void)
{
	usleep(200000);
}



int send_read(int fd,char*cmd,char* read_str,int wait_sign)
{
	int sign_display=1;
	int j;
	char cmd_buf[100];
	char buf_echo[30];
	char*p_temp;
	char buf_temp[100];
	int n=0;
	char*p1,*p2;	
        char*p;
        int i;
        char buf[1024];
        int readsize;
        int ret=false;
        if(read_str==NULL) return ret;
	p1=buf;

	strcpy(buf_echo,"echo \"at\" > ");
	strcat(buf_echo,para_in[0]);

	strcpy(cmd_buf,cmd);
	cmd=cmd_buf;
      p=cmd;
        p+=3;
        i=0;

	if((p_temp=strchr(p,'='))||(p_temp=strchr(p,'?'))||(p_temp=strchr(p,0x0d)))	//0x0d= \r
	{
		memset(buf_temp,0,sizeof(buf_temp));
		n=p_temp-p;
		strncpy(buf_temp,p,n);
		//if(!strcmp(buf_temp,"GSN")) strcpy(buf_temp,"GSN");
	}


#ifdef DEBUG_DISPLAY_READ
        printf("write:%s",cmd);
        printf("end cmd\n");
#endif
		system(buf_echo);
		system(buf_echo);
		sign_display=0;		
tcflush(fd,TCIOFLUSH);
//tcflush(fd,TCIOFLUSH);
        write(fd,cmd,(strlen(cmd)));
	i=0;
        while(1)
        {
                memset(buf,0,sizeof(buf));
        //       buf[0]=0;
//printf("--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 start read\n");
                readsize=read(fd,buf,sizeof(buf));
//printf("--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 end  read\n");
                buf[readsize]=0;
tcflush(fd,TCIOFLUSH);
delay();
#ifdef DEBUG_DISPLAY_READ
//printf("\n\n\n\n\n");
//printf("--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 display\n");
//	
        printf("~~~~~~~~~~~~~~~read_all buf:%s\n~~~~~~~~~~~~read size:%d\n",buf,readsize);
//printf("--!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 end  display\n");
//printf("\n\n\n\n\n");
#endif


//tcflush(fd,TCIOFLUSH);
                if(wait_sign==WAIT_VALUE)
                {
			
                     	//for(i=0;i<10;i++)
                     	i=0;
                     	while(i<8)
                     	{
				//if(buf[0]=='A') break;
				if((buf[i]!=0x0d)&&(buf[i]!=0x0a))
				{
					p1=&buf[i];
					break;	
				}
				i++;
			}
#ifdef DEBUG_DISPLAY_READ
     //   printf("p1:%s\n",p1);
#endif
			if((p1!=NULL)&&((*p1!='^')||(*p1!='+')||(*p1!='%')))
			{
				if((*p1=='A')&&(*(p1+1)=='T')&&(*(p1+2)=='^'))
				{
					p1=strchr(p1+3,'^');
					if(p1==NULL) p1=buf;
				}
				else if((*p1=='A')&&(*(p1+1)=='T')&&(*(p1+2)=='+'))
				{
					p1=strchr(p1+3,'+');
					if(p1==NULL) p1=buf;
				}
				else if((*p1=='A')&&(*(p1+1)=='T')&&(*(p1+2)=='%'))
				{
					p1=strchr(p1+3,'%');
					if(p1==NULL) p1=buf;
				}
			}
                        //if(((*p1=='^')||(*p1=='+'))&&(*p++==*(p1+1))&&(*p==*(p1+2)))
   //    printf("send read:p1:%s,buf_temp:%s\n",p1,buf_temp);
#ifdef DEBUG_DISPLAY_READ
     //   printf("p1:%s\n",p1);
#endif
#if defined(MODULE_MU103)
			if(strstr(buf_temp,"CGMR")||strstr(buf_temp,"CGSN"))
			{
				if((strlen(p1)>15)&&(*p1>='0')&&(*p1<='9'))  
				{
                                	#ifdef DEBUG_DISPLAY_READ
	                                 printf("read buf p1:%s\n",p1);
        	                        #endif
					strcpy(buf,p1);
					p2=buf;	
					while(*p2!=0) 
					{
						if((*p2==0x0d)||(*p2==0x0a)) *p2=0;	
						p2++;
					} 
	                                #ifdef DEBUG_DISPLAY_READ
        	                         printf("handled buf:%s\n",buf);
                	                #endif
                        	     ret=true;
                                	break;
						
				}		
			}
#endif
                        if(((*p1=='^')||(*p1=='+')||(*p1=='%'))&&(strstr(p1,buf_temp)))
                        {
				
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("read buf:%s\n",buf);
                                #endif
				p2=p1;	
				while(*p2!=0) 
				{
					if((*p2==0x0d)||(*p2==0x0a)) *p2=0;	
					p2++;
				} 
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("handled buf:%s\n",buf);
                                #endif
                             ret=true;
                                break;
                         }

#ifdef DEBUG_DISPLAY_READ
      //  printf("p1:%s\n",p1);
#endif
#if defined(MODULE_U6300)||defined(MODULE_EM770)
		    if(strstr(cmd,"AT+CGMR")|| strstr(cmd,"AT+CGSN")) //not dusimr command
                        {
				if(strstr(buf,"OK"))
				{
						p2=p1;	
						while(*p2!=0) 
						{
							if((*p2==0x0d)||(*p2==0x0a)) *p2=0;	
							p2++;
						} 
						strcpy(buf,p1);
                                        #ifdef DEBUG_DISPLAY_READ
                                         printf("read buf:%s\n",buf);
                                        #endif
                                        ret=true;
                                        break;
	
				}
                                if(strstr(buf,"+CMS ERROR") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER")|| strstr(buf,"COMMAND NOT SUPPORT"))
                                {
                                        #ifdef DEBUG_DISPLAY_READ
                                         printf("read buf:%s\n",buf);
                                        #endif
                                        ret=false;
                                        break;
                                }
                        }
#endif


                        if(strstr(cmd,"AT+CMGL")||strstr(cmd,"AT^HCMGL")||strstr(cmd,"AT+CGEQNEG")) //not dusimr command
                        {
				if(strstr(buf,"OK"))
				{
                                        #ifdef DEBUG_DISPLAY_READ
                                         printf("read buf:%s\n",buf);
                                        #endif
                                        ret=true;
                                        break;
	
				}
                                if(strstr(buf,"+CMS ERROR") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER")|| strstr(buf,"COMMAND NOT SUPPORT"))
                                {
                                        #ifdef DEBUG_DISPLAY_READ
                                         printf("read buf:%s\n",buf);
                                        #endif
                                        ret=false;
                                        break;
                                }
                        }


			if(strstr(buf,"ERROR"))
			{
				ret=false;
				break;	
			}

                }
                else if(wait_sign==WAIT_OK)
                {
			if(strstr(buf,"OK"))
                	{
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("read buf:%s\n",buf);
                                #endif
                        	ret=true;
				break;
                	}
			if(strstr(buf,"+CMS ERROR") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER") || strstr(buf,"COMMAND NOT SUPPORT"))
                	{
                                #ifdef DEBUG_DISPLAY_READ
                                 printf("read buf:%s\n",buf);
                                #endif
                        	ret=false;
				break;
                	}



                }
		else if(wait_sign==WAIT_NONE) //for issue at command
		{
		
	              // if((buf[0]!='\n')&& !strstr(buf,"^HRSSILVL") &&  !strstr(buf,"^RSSILVL"))
	               if((!strstr(buf,"^HRSSILVL")) && ( !strstr(buf,"^RSSILVL")))
                        {
				if(sign_display==1)
				{
                                printf("%s",buf);
                                printf("\n");
                             	//ret=true;
                             	}
                        }
                        if(strstr(buf,"OK") || strstr(buf,"ERROR")|| strstr(buf,"NO CARRIER") || strstr(buf,"COMMAND NOT SUPPORT"))
			{
				
				ret=true;
				 break;
			}

		}
		
             if(i++>30) break;
         }

        if(ret==true)
        {
                strcpy(read_str,buf);
        }
        else
        {
                strcpy(read_str,"");
        }
        return ret;
}















int main(int argc,char* argv[])
{

	int ret=true;
	char ch;
	char *p,*p1;
	char buf[100];
	char temp[50];
	char str[1000];

       signal(SIGALRM,timeout_handle);
       alarm(TIMEOUT_TIME);



	if((argc==1))
	{
		printf("usage: 3Ginfo [-d]  [-c] [-s]\n");
		return -1;
	}



	//openlog(LOG_IDENT,LOG_PID,LOG_USER);

	//chdir(SDK_FORDER);





	memset(para_in[0],0,sizeof(para_in[0]));
	memset(para_in[1],0,sizeof(para_in[1]));
	memset(para_in[2],0,sizeof(para_in[2]));
	memset(para_in[3],0,sizeof(para_in[3]));

	
	while((ch=getopt(argc,argv,"d:c:s:m:"))!=(char)-1)
	{
		switch(ch)
		{
			case 'd':
				if(optarg!=NULL)
				{
					strcpy(para_in[0],optarg);
				}
			break;
			case 'c':
				if(optarg!=NULL)
				{
					strcpy(para_in[1],optarg);
				}
					
			break;
			case 's':
				if(optarg!=NULL)
				{
					strcpy(para_in[2],optarg);
				}
			break;
			case 'm':
				if(optarg!=NULL)
				{
					strcpy(para_in[3],optarg);
				}
			break;
			default:
				printf("error command");
				ret=false;
				exit(0);
			break;
		}
	}	




	p=default_command;
	

	if(strlen(para_in[0])==0)
			strcpy(para_in[0],default_device);
	if(strlen(para_in[1])==0)
			strcpy(para_in[1],default_command);
	if(strlen(para_in[2])==0)
			strcpy(para_in[2],default_sign);
	if(strlen(para_in[3])==0)
			strcpy(para_in[3],default_max_signal);

//printf("para_in[0]=%s,strlen=%d\n",para_in[0],strlen(para_in[0]));
//printf("para_in[1]=%s,strlen=%d\n",para_in[1],strlen(para_in[1]));
//printf("para_in[2]=%s,strlen=%d\n",para_in[2],strlen(para_in[2]));

	strcat(para_in[1],"\r\n");

	serial_init(para_in[0]);

//system("echo \"open 3g device for get signal \"  ");
	if(open_device(&fd,para_in[0])==false)
	{
		printf("open device error!");
		ret=false;
		 exit(0);
	}

		
//system("echo \"!!!!!!!send ate0 \"  ");
	send_read(fd,"ATE0\r\n",buf,WAIT_OK);
//	sprintf(str,"echo \"send %s\n\" ",para_in[0]);
//system(str);
	if(send_read(fd,para_in[1],buf,WAIT_VALUE)==false) 
	{
//		system("echo \"ret=false\" ");
		ret=false;
		printf("read error!");
		close_device(fd);
		clear_fd(&fd);
		exit(0);
        }
	

//sprintf(str,"echo \"buf %s\n\" ",buf);
//system(str);
//printf("last_buf:%s\n",buf);
               

		if(buf==NULL) 	
		{
			printf("read error!");
			ret=false;
			close_device(fd);
			clear_fd(&fd);
			exit(0);
		}
		//ret
		close_device(fd);
		clear_fd(&fd);
			








		p=strchr(buf,':');
		if(p!=NULL)
		{
			p++;
			p1=strchr(buf,',');
			if(p1!=NULL)
			{
				strncpy(temp,p,p1-p);
				printf("%s , ",temp);
				printf("(");
				printf("%s",para_in[3]);
				printf(")");
			}
			else{
				printf("");	
			}
		}
		else{
			printf("");	
		}

		exit(0);
}


