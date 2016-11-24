#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>

struct sembuf unlock[1] = {0,1,0};
struct sembuf lock[1] = {0, -1, 0};
int main()
{

    int memd = shmget(1568,0,0);
    if (memd ==-1){
        perror("not today");
        return 2;
    }

    char *addr=(char*)shmat(memd,0,0);
    if (addr == (char*)(-1)) {
	perror("shmat");
	return 2;
    }
    memset(addr,'\0',sizeof(addr));
//    printf("[CLIENT]: connected & ready to write;\n");
    FILE* fd = popen("users","r");
    char buf_user[100];
    while(!feof(fd)){
        memset(buf_user,'\0',sizeof(buf_user));
        fscanf(fd,"%s",buf_user);
        strcat(addr,buf_user);
        strcat(addr,"\n");
    }
    int semd = semget(1566,1,0);
  //  printf("[CLIENT]: server was unlocked\n");
    semop(semd,unlock,1);
 //   printf("[CLIENT]: client was locked\n");
    semop(semd,lock,1);
    printf("[Client]: %s \n",addr);
    semop(semd,unlock,1);
    return 0;
}
