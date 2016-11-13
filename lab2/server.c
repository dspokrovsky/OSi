#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <string.h>

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
    printf("[SERVER: locked]\n");
    semop(semd,lock,1);
    printf("[SERVER: unlocked]\n");
    // time & last pr >>> write in addr
    printf("[SERVER]:%s\n",addr);

    //memset(addr,'\o',sizeof(addr));


    //last pr
    if (semctl(semd,0,GETPID,arg) == -1) perror("semctl ");
    arg.buf->sem_otime;




    //del
    if (semctl(semd,0,IPC_RMID,0) == -1) perror("semctl rmid");
 return 0;
}
