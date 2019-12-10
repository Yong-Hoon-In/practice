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
    int str_len;
    char *startmsg="가위바위보 게임을 시작합니다\n";
    if (argc != 3)
    {
        printf("Usage : %s <IP> <Port>\n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");
	

    printf("%s",startmsg);
    while(1){
        read(sock, message, BUF_SIZE);
        printf("%s",message);
	while(1){
        scanf("%s",message);
	if(strcmp(message,"quit")==0)
	return 0;
	else if((strcmp(message,"가위")!=0)&&(strcmp(message,"바위")!=0)&&(strcmp(message,"보")!=0)) 
		printf("다시 입력해주세요");
	
	}
        write(sock, message, BUF_SIZE);
        read(sock, message, BUF_SIZE);
        printf("%s",message);
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

