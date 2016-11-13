#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>

struct sembuf op = {0,1,0};
int main()
{

    int memd = shmget(1567,1024,0);
    if (memd ==-1){
        perror("not today");
        return -2;
    }
    char *addr=(char*)shmat(memd,0,0);//
    memset(addr,'\0',sizeof(addr));
    FILE* fd = popen("who","r");
    char buf_user[100];
    while(!feof(fd)){ // !feof(fd)
        memset(buf_user,'\0',sizeof(buf_user));
        fscanf(fd,"%s",buf_user);
        strcat(addr,buf_user);
        strcat(addr,"\n");
    }
    int semd = semget(1567,1,0);
    semop(semd,&op,1);

    printf("%s\n",addr);


    return 0;
}
