#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <ucontext.h>
 
 

int main(int argc, char** argv)
{
	int ret = 1; 
    int count = 0;
   int i;

    int spi1=10;
    int spi2=10;
  /*  char buf[30];
    memset(buf,0,sizeof(buf));

    FILE*fp;
    if((fp=popen("cat /var/stop_check_vpn.log","r"))==NULL){
		return -1;
	}
    fgets(buf, sizeof(buf), fp);

    if(buf[0] == '1')
    {
         system("echo \"is not online\">>/var/vpn1.log");
         if(fp) pclose(fp);
         exit(0);
    }
    
    if(fp) pclose(fp);*/
   
   
 
    system("echo \"check vpn\">>/var/vpn1.log");
	//while(1)
	{
 
        system("echo \"check vpn......1\">>/var/vpn1.log");
		ret = system("ps>/var/ps.log");
        system("echo \"check vpn......2\">>/var/vpn1.log"); 
        ret = system("cat /var/ps.log|grep start_vpn");//check start_vpn
        system("echo \"check vpn......3\">>/var/vpn1.log");
		if(ret != 0)//is not find start_vpn ,so need to start_vpn
		{
            system("echo \"check vpn......4\">>/var/vpn1.log");
			printf("can not find start_vpn process, restart it!\n");
			system("start_vpn&");
             exit(1);
		}
		else//check spi
		{
            system("echo \"check vpn......5\">>/var/vpn1.log");
            printf("start_vpn is ok\n");
        

             
                
                        ret = system("setkey -D>/var/spi1.log");
                        if(ret != 0)
                            return 0;
                        
             
                
         
            if(system("cat /var/spi1.log|grep \"No SAD entries.\"") == 0)//find the key word,so need start_vpn
			{
                system("echo \"check vpn......6\">>/var/vpn1.log");
				printf("can not establish ipsec tunnel, restart it\n");
				//system("echo `setkey -D`>>/var/vpn.log");
				system("setkey -FP");
				system("killall start_vpn");
				system("start_vpn&");	
                
                exit(1);
			}
            sleep(1);

            if((system("cat /var/spi1.log|grep \"spi=0(0x00000000)\"")==0))
			{
                system("echo \"check vpn......6\">>/var/vpn1.log");
				printf("can not establish ipsec tunnel, restart it\n");
				//system("echo `setkey -D`>>/var/vpn.log");
				system("setkey -FP");
				system("killall start_vpn");
				system("start_vpn&");	
                
                 exit(1);
			}
             sleep(1);
             if((system("cat /var/spi1.log|grep \"spi=0(0x00000000)\"")==0))
			{
                system("echo \"check vpn......6\">>/var/vpn1.log");
				printf("can not establish ipsec tunnel, restart it\n");
				//system("echo `setkey -D`>>/var/vpn.log");
				system("setkey -FP");
				system("killall start_vpn");
				system("start_vpn&");	
                
                 exit(1);
			}
            else
                {
                    system("echo \"check vpn......7\">>/var/vpn1.log");
                     count = 0;
                    printf("success\n");
                    exit(0);
                }
		}
	}
    exit(1);
	return 0;
}
