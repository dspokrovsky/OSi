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
    printf("[SERVER]: locked]\n");
    semop(semd,lock,1);
    printf("[SERVER]: unlocked]\n");
    char cmd [1000];
    memset(cmd,'\0', sizeof(cmd));
    strcpy(cmd, "n=$(who | wc -l)\nwhile [ $n != 0 ]\ndo\necho $(who | head -n $n |tail -n 1 |awk '{print $1}') $(date -d@$(( $(date -ud now +%s) - $(date -ud \"$(who | head -n $n |tail -n 1 |awk '{print $3,$4}')\" +%s) )) +'%H:%M')\nlet \" n = $n - 1 \" \ndone");
    // time & last pr >>> write in addr
    printf("[SERVER]: go \n");
    FILE* fd = popen(cmd,"r");
    printf("[SERVER]: go+ \n");
    char i;
    int j =0;
    while (!feof(fd)){
        i = fgetc(fd);
        addr[j]=i;
        j++;
    }
    j--;
    addr[j]='\0';
    printf("[SERVER]: %s \n",addr);

    //time last process TODO
    //if (semctl(semd,0,GETPID,arg) == -1) perror("semctl ");
    //printf("%ju\n",arg.buf->sem_otime);
    printf("[SERVER]: give id to client \n");
    semop(semd,unlock,1);
    semop(semd,lock,1);
    //del
    if (semctl(semd,0,IPC_RMID,0) == -1) perror("semctl rmid");
    if (shmctl(memd,IPC_RMID,0) == -1) perror("semctl rmid");
 return 0;
}
