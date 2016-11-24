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


int main()
{
    struct semid_ds buf;
    arg.buf=&buf;
    int memd = shmget(1568, 1024, IPC_CREAT | 0664);
    char *addr=(char*)shmat(memd,0,0);
    if(addr ==(char*)-1){
        perror("Shmat error");
        return 1;
    }
    //sema
    int semd = semget(1566, 2, IPC_CREAT | 0664);
    if (semd == -1)
    {
        printf("Ошибка в semget\n");
        return 1;
    }
    //printf("[SERVER]: locked]\n");
    semop(semd,lock,1);
    //printf("[SERVER]: unlocked]\n");
    FILE* fd = popen("./t","r");
    char i;
    int j =0;
    while (!feof(fd)){
        i = fgetc(fd);
        addr[j]=i;
        j++;
    }
    j--;
    addr[j]='\0';
    //printf("[SERVER]: %s \n",addr);
    //time last process
    if (semctl(semd,0,IPC_STAT,arg) == -1) {perror("semctl "); return 2;}
    strcat(addr, asctime(localtime(&arg.buf->sem_otime)));
    //printf("[SERVER]: give time to client \n");
    semop(semd,unlock,1);
    semop(semd,lock,1);
    //del
    if (semctl(semd,0,IPC_RMID,0) == -1) perror("semctl rmid");
    if (shmctl(memd,IPC_RMID,0) == -1) perror("shmctl rmid");
 return 0;
}
