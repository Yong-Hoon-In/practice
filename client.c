#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#define BUF_SIZE 200
void error_handling(char *message);
int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    char value[BUF_SIZE];
    int str_len;
    if (argc != 3)
    {
        printf("Usage : %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
    else
        puts("Connected");

    read(sock,message,BUF_SIZE);
    printf("%s", startmsg);
    printf("%s",message);
    while (1)
    {
        scanf("%s",&value);
        write(sock,value,sizeof(value));
        if(value==0){
            printf("낸것 :가위");
        }
        if(value==1){
            printf("낸것 :바위");
        }
        if(value==2){
            printf("낸것 :보");
        }
    }

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
