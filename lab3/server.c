#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


int main()
{
printf("[SERVER]: start\n");
int sid = socket (AF_UNIX,SOCK_DGRAM,0);
if (sid ==-1){
	perror("socket cl");
	return 2;
}
struct sockaddr_un serv_addr;
memset(&serv_addr,0,sizeof(serv_addr));
serv_addr.sun_family = AF_UNIX;
strcpy(serv_addr.sun_path,"sock2");
struct sockaddr_un client_addr;
memset(&client_addr,0,sizeof(client_addr));
client_addr.sun_family = AF_UNIX;
strcpy(client_addr.sun_path,"client_sock2");

//unlink("sock2");
if(bind(sid,(const struct sockaddr*)&serv_addr,sizeof(serv_addr))!=0){
	perror("bind serv");
	return 2;
}
FILE *fd = popen("ls -a | wc -l","r");
int count;
fscanf(fd,"%d", &count);
printf("[SERVER]: files %d\n",count);
int secs = 2;
while(sendto(sid,&count,sizeof(&count),0, (const struct sockaddr * )&client_addr, 
sizeof(struct sockaddr_un)) <0){
	perror("sendto");
	sleep(1);
	if (secs-- ==0){
		printf("[SERVER]: ou of time \n");
		return 2;
	}
}
int len = sizeof(struct sockaddr_un);
if (recvfrom(sid,&count,sizeof(&count),0,(struct sockaddr*)&client_addr, 
&len)==1){
perror("recvFrom");
return 23112;
}
printf("[SERVER]data:%d",count);
close(sid);
unlink("sock2");
return 2;
}
