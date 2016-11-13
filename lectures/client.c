#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(){
    struct sockaddr_in kaddr;
    struct hostent *hp;
    int fd;
    char buf[2048];
    fd = socket(AF_INET, SOCK_STREAM,0);
    bzero((void *)&kaddr,sizeof(kaddr));
    hp = gethostbyname('127.0.0.1');
    bcopy(hp->h_addr_list,(struct sockaddr*)&kaddr.sin_addr,hp->h_length);

    kaddr.sin_addr = inet_addr("127.0.0.1");
    kaddr.sin_port = ktons(8000);

    kaddr.sin_family =AF_INET;
    if (connect(fd,(struct sockaddr*)&kaddr,sizeof(kaddr))!=0){
        perror("error to connect\n");
        return -1;
    }

    read(fd,buf,sizeof(buf));
    send(fd,buf,2048,0);


    return 0;
}
