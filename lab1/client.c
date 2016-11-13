/*Сервер.
 * Создать очередь сообщений.
 * Записать в неё сообщение об именах файлов,
 * содержащих командный текст.
 * Клиент.
 * Выбрать из очереди сообщений последнее сообщение.
 *  Отфильтровать в нем те файлы,
 *  в которых осуществляется обработка
 * прерываний и записать их в стандартный файл вывода.
 *  Вывести также значение
 * идентификатора процесса,
 *  который последним передал сообщение в очередь.
 * Удалить очередь сообщений.
*/


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>

#define MSGMAX 1024
#define MTYPE 8

struct msgbuf{
    long mtype ;
    char text[MSGMAX];
}buf_msg;

int main(){
    int fd=-1,i=0,z=10;
    fd=msgget(1567,0);
    while(fd==-1)
    {
        if (z==0) return 0;
        sleep(2);
        fd=msgget(1567,0);
        if(fd!=-1) break;
        z--;
    }
    printf("[Client]: connected to msg queue\n");

    if (msgrcv(fd, &buf_msg, MSGMAX, MTYPE, IPC_NOWAIT | MSG_NOERROR)==-1){
        perror("[Client]: failed to read the messege.\n");
        return 0;
    }
    char cmd[MSGMAX];
    char name[MSGMAX];
    memset(name,'\0',MSGMAX);
    memset(cmd,'\0',MSGMAX);
    FILE* fp = fopen("buf.temp","w+");
    fprintf(fp,"%s",buf_msg.text);
    fclose(fp);
    FILE* fp2 = fopen("buf.temp","r");

    if(!fp2){
        perror("Error fp2");
    }
    while ( !feof(fp) ){
        memset(name,'\0',MSGMAX);
        memset(cmd,'\0',MSGMAX);
        fscanf(fp2,"%s",name);
        if (name[0]== '\0') break;
        strcat(cmd,"grep 'trap' ");
        strcat(cmd, name);
        strcat(cmd, " | wc -l ");
        FILE* fp3 = popen(cmd,"r");
        int val[1];
        fscanf(fp3,"%d",val);
        if(!val[0]) printf("[Client]: %s\n",name);
        fclose(fp3);
    }
    struct msqid_ds queue;
    msgctl(fd, IPC_STAT, &queue);
    int lastid = queue.msg_lspid;
    printf("[Client]: Pid of last: %d \n", lastid);
    if (msgctl(fd, IPC_RMID,0)==-1)
        perror("[Client]: Failed to remove the queue.\n");
    else
        printf("[Client]: The queue has been removed.\n");
    return 0;
}
