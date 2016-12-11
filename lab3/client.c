#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
int main(){
printf("[CLIENT]: go\n");
int sid = socket (AF_UNIX,SOCK_DGRAM,0);
if (sid ==-1){
	perror("socket");
	return 2;
}
struct sockaddr_un serv_addr;
memset(&serv_addr,0,sizeof(serv_addr));
serv_addr.sun_family = AF_UNIX;
strcpy(serv_addr.sun_path,"sock2");

struct sockaddr_un client_addr;
memset(&client_addr,0,sizeof(client_addr));
client_addr.sun_family =AF_UNIX;
strcpy(client_addr.sun_path,"client_sock2");
//unlink("client_sock2");
if(bind(sid,(const struct sockaddr*)&client_addr,sizeof(client_addr))!=0){
	perror("bind cl");
	return 2;
}
int len = sizeof(struct sockaddr_un);
int count;
recvfrom(sid,&count,sizeof(&count),0,(struct sockaddr * ) &serv_addr,&len);
printf("[CLIENT]: recived: %d\n",count);
FILE *fd = popen("ls -a -1 | grep \'^\\.\'|wc -l","r");
int count2;
fscanf(fd,"%d", &count2);
printf("CLIENT: now files%d \n",count2);
count = count - count2;
printf("CLIENT: sended %d\n",count);
while (sendto(sid,&count,sizeof(&count),0,(struct sockaddr*)&serv_addr,sizeof(struct 
sockaddr_un))<0){
	perror("sendto");
	sleep(1);
}
close(sid);
unlink("client_sock2");
printf("[CLENT]: gl hf\n");
return 0;
}
