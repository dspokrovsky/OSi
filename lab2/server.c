#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>
#include <time.h>
union semun
{
    int val;
    struct semid_ds *buf;
    ushort *array;
} arg;

struct sembuf unlock[1] = {0, 1, 0};
struct sembuf lock[1] = {0, -1, 0};
struct sembuf check[1] = {0, 0, 0};

int main()
{
    int memd = shmget(1567, 1024, IPC_CREAT | 0664);
    char *addr=(char*)shmat(memd,0,0);
    if(addr ==(char*)-1){
        printf("shmat error");
        return 1;
    }
    //sema
    int semd = semget(1567, 2, IPC_CREAT | 0664);
    if (semd == -1)
    {
        printf("Ошибка в semget\n");
        return 1;
    }
    printf("[SERVER]: locked]\n");
    semop(semd,lock,1);
    printf("[SERVER]: unlocked]\n");

    char cmd [1000];
    memset(cmd,'\0', sizeof(cmd));

    strcpy(cmd,"who | grep -E '");
    strcat(cmd,addr);
    memset(addr,'\0', sizeof(addr));
    strcat(cmd,"' |awk '{print $1,$4}'");

    // time & last pr >>> write in addr
    printf("[SERVER]:%s\n",cmd);

    FILE* fd = popen(cmd,"r");
    char i;
    int j =0;

    while (!feof(fd)){
        i = fgetc(fd);
        addr[j]=i;
        j++;
    }

    j--;
    addr[j]='\0';


    //time last process
    if (semctl(semd,0,GETPID,arg) == -1) perror("semctl ");
    //time_t t = arg.buf->sem_otime;


    semop(semd,unlock,1);
    semop(semd,lock,1);
    //del
    if (semctl(semd,0,IPC_RMID,0) == -1) perror("semctl rmid");
    if (shmctl(memd,IPC_RMID,0) == -1) perror("semctl rmid");
 return 0;
}
