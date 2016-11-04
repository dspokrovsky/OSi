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

#define MSGMAX 1024
#define MTYPE 8

#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

struct msgbuf
{
    long mtype;
    char text[MSGMAX];
} buf_msg;


int main()
{
    int fd = msgget(1567, IPC_CREAT | 0664);
    if (fd == -1)
    {
        perror("Can't create an queue\n");
        return 1;
    }
    printf("[SERVER] An queue has been created.\n");

    char buf[100];
    memset(buf,'\0',100);
    memset(buf_msg.text,'\0',MSGMAX);
    FILE* fp = popen("ls | file -f - | grep 'shell script' - | awk -F': ' '{print $1}'","r");
    long x = 0,x2 =0;
    while ( ftell(fp) != x2 ){
        fscanf(fp,"%s",buf);
        strcat(buf_msg.text, buf);
        strcat(buf_msg.text, "\n");
        x2 = x;
        x = ftell(fp);
    }
    printf("[SERVER]: message: %s ==end\n",buf_msg.text);
    buf_msg.mtype = MTYPE;
    if (msgsnd(fd, &buf_msg, MSGMAX, IPC_NOWAIT) == -1)
    {
        perror("[SERVER] Failed to send a message \n");
        return 2;
    }
    return 0;
}
