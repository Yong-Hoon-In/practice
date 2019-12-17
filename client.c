#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#define BUF_SIZE 1024
void error_handling(char *message);
int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    char value[BUF_SIZE];
    int str_len;
    int read_cnt;
    int draw=0,lose=0,win=0;
    int i;
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

    read(sock, message, BUF_SIZE);
    printf("가위바위보 게임을 시작합니다\n");
    printf("%s\n", message);

    
    read(sock, message, BUF_SIZE);
	for(i=0;i<BUF_SIZE;i++){
	    if(message[i]=='0')
		draw++;
	    else if(message[i]=='1')
		lose++;
	    else if(message[i]=='2')
		win++;
	    else
		continue;
	}
    
printf("당신의 전적은 ::: \n이긴 횟수: %d 진 횟수: %d 비긴 횟수: %d\n",win,lose,draw);


    
    
    while (1)
    {
        scanf("%s", value);
        write(sock, value, sizeof(value));
        if (strncmp(value, "quit", 3) == 0)
        {
            puts("연결을 종료합니다.");
            close(sock);
            exit(0);
        }
        else if (strncmp(value, "0", 1) == 0)
        {
            printf("낸것 :가위\n");
            read(sock, message, BUF_SIZE);
            printf("%s", message);
        }
        else if (strncmp(value, "1", 1) == 0)
        {
            printf("낸것 :바위\n");
            read(sock, message, BUF_SIZE);
            printf("%s", message);
        }
        else if (strncmp(value, "2", 1) == 0)
        {
            printf("낸것 :보\n");
            read(sock, message, BUF_SIZE);
            printf("%s", message);
        }
        else
        {
            printf("잘못입력하셧습니다");
            read(sock, message, BUF_SIZE);
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
