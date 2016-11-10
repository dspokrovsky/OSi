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

int main()
{
    int md = shmget(1567, 1024, IPC_CREAT | 0664);
    char *addr=(char*)shmat(fd,0,0);
    if(addr ==(char*)-1){
        perror("shmat");
        return 1;
    }
    //sema
    int semd = semget(1567, 2, IPC_CREAT | 0664);
    if (semd == -1)
    {
        printf("Ошибка в semget\n");
        return 1;
    }


    // time & last pr >>> write in addr
    struct sembuf sem_bl[1] = {1, 1, 0};
    semop(semd,sem_bl,1);


    //last pr
    if (semctl(semd,0,GETPID,arg) == -1) perror("semctl ");
    arg.buf->sem_otime;




    //del
    if (semctl(semd,0,IPC_RMID,0) == -1) perror("semctl rmid");
 return 0;
}
