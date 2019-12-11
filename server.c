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
    socklen_t clnt_addr_size;

    char informsg[] = "'가위(0)' 또는 '바위(1)' 또는 '보(2)' 를 입력해주세요\n 끝내려면 quit 를 입력해주세요\n";
    char win[] = "축하합니다 이겼습니다!\n";
    char loss[] = "졌습니다! 다시 도전해 보세요\n";
    char tie[] = "비겼습니다! 다시 도전해 보세요\n";
    char bawi[] = "바위";
    char gawi[] = "가위";
    char bo[] = "보";
    char server[] = "서버는";
    char client[] = "를 냈고 클라이언트는";
    char msg[] = "를 냈습니다 \n:";
    char buffer[BUF_SIZE] = {0};
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

    clnt_addr_size = sizeof(clnt_addr);

    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error");
    printf("가위바위보 시작");

    while (1)
    {
        char buffer[BUF_SIZE];
        memset(buffer, 0, sizeof(buffer));
        write(clnt_sock, informsg, sizeof(informsg));

        read(clnt_sock, buffer, BUF_SIZE);
        int client_num = atoi(buffer);
        srand((unsigned)time(NULL));
        int server_num = rand() % 3;

        usleep(100);
        if (server_num == 0)
        {
            fputs(gawi, stdout);
            write(clnt_sock, gawi, sizeof(gawi));
            usleep(100);
        }
        else if (server_num == 1)
        {
            fputs(bawi, stdout);
            write(clnt_sock, bawi, sizeof(bawi));
            usleep(100);
        }
        else
        {
            fputs(bo, stdout);
            write(clnt_sock, bo, sizeof(bo));
            usleep(100);
        }
        fputs(client, stdout);
        write(clnt_sock, client, sizeof(client));
        usleep(100);
        if (client_num == 0)
        {
            fputs(gawi, stdout);
            write(clnt_sock, gawi, sizeof(gawi));
            usleep(100);
        }
        else if (client_num == 1)
        {
            fputs(bawi, stdout);
            write(clnt_sock, bawi, sizeof(bawi));
            usleep(100);
        }
        else
        {
            fputs(bo, stdout);
            write(clnt_sock, bo, sizeof(bo));
            usleep(100);
        }
        fputs(msg, stdout);
        write(clnt_sock, msg, sizeof(msg));
        usleep(100);

        result = who_win(server_num, client_num);
        if (result == 0)
        {
            write(clnt_sock, no_winner, sizeof(no_winner));
            fputs(no_winner, stdout);
        }
        else if (result == 1)
        {
            write(clnt_sock, lose, sizeof(lose));
            fputs(win, stdout);
        }
        else
        {
            write(clnt_sock, win, sizeof(win));
            fputs(lose, stdout);
        }

        puts("유저와의 연결을 종료합니다.");
        close(clnt_sock);
        exit(0);
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
int who_win(int a, int b)
{
    if (a == b)
    {
        return 0;
    }
    else if ((a == 0 && b == 2) || (a == 1 && b == 0) || (a == 2 && b == 1))
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
