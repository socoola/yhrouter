#include <stdio.h>
#include <string.h>
#include <netdb.h>






int main(int argc,char*argv[])
{
	struct hostent *host;
	struct sockaddr_in addr;	
	char buf[100];	
	char ip[20];

	memset(buf,0,sizeof(buf));
	memset(ip,0,sizeof(ip));
	
	if((host=gethostbyname(argv[1]))==NULL){
		printf("error!");	
		return 0;
	}
	memset(&addr,0,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr=*((struct in_addr*)host->h_addr);
	strcpy(ip,(char*)inet_ntoa(addr.sin_addr));

	printf("%s",ip);	
	return 0;
}
