#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <string.h>
#define BUF_SIZE 200
void error_handling(char *message);
int Rock(int msg);
int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock;
    int value = 0;
    int com = 0;
    struct sockaddr_in serv_addr, clnt_addr;
    char ttt[20];
    socklen_t clnt_addr_size;

    char informsg[] = "'가위' 또는 '바위' 또는 '보' 를 입력해주세요\n 끝내려면 quit 를 입력해주세요\n";
    char win[] = "축하합니다 이겼습니다!\n";
    char loss[] = "졌습니다! 다시 도전해 보세요\n";
    char tie[] = "비겼습니다! 다시 도전해 보세요\n";
    char clntmsg[BUF_SIZE];
    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");
    while (1)
    {
        clnt_addr_size = sizeof(clnt_addr);

        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        while (1)
        {
            write(clnt_sock, informsg, sizeof(informsg));
            read(clnt_sock, clntmsg, BUF_SIZE);

            if (strcmp(clntmsg, "quit") == 0)
            {
                close(clnt_sock);
                break;
            }
            if (strncmp(clntmsg, "가위", 1) == 0)
                value = 0;
            else if (strncmp(clntmsg, "바위", 1) == 0)
                value = 1;
            else if (strncmp(clntmsg, "보", 1) == 0)
                value = 2;

            com = Rock(value);
            if (com == 0)
                strncpy(ttt, "가위", strlen("가위"));
            if (com == 1)
                strncpy(ttt, "바위", strlen("바위"));
            if (com == 2)
                strncpy(ttt, "보", strlen("보"));

            printf("사용자가 낸것: %s 서버에서 낸것: %s", clntmsg, ttt);

            if (value = 1)
                write(clnt_sock, win, BUF_SIZE);
            else if (value = -1)
                write(clnt_sock, loss, BUF_SIZE);
            else if (value = 0)
                write(clnt_sock, tie, BUF_SIZE);
        }
        printf("Finish\n");
    }

    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
int Rock(int msg)
{
    srand((unsigned)time(NULL));
    int ran = rand() % 3;
    if (msg == ran)
        return 0; //무승부
    else if ((msg == 0 && ran == 2) || (msg == 1 && ran == 0) || (msg == 2 && ran == 1))
        return 1; //이김
    else
        return -1; //짐
    return ran;
}
