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
int who_win(int a, int b);
int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock, i;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size;
    int str_len;
    char informsg[] = "'가위(0)' 또는 '바위(1)' 또는 '보(2)' 를 입력해주세요\n 끝내려면 quit 를 입력해주세요\n";
    char win[] = "축하합니다 이겼습니다!\n";
    char lose[] = "졌습니다! 다시 도전해 보세요\n";
    char tie[] = "비겼습니다! 다시 도전해 보세요\n";
    char bawi[] = "바위";
    char gawi[] = "가위";
    char bo[] = "보";
    char one[]="1";
    char two[]="2";
    char zero[]="0";
    char buffer[BUF_SIZE];
    int value, com;
    char message[BUF_SIZE];
    FILE *fp;
    int read_cnt;
    char buf[BUF_SIZE];
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

    int random;
    clnt_addr_size = sizeof(clnt_addr);

    for (i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
            error_handling("accept() error");
        else
            printf("Connected client");


        fp = fopen("option.txt", "wb");
        if (fp != NULL)
        {
            while (1)
            {
                read_cnt = fread((void *)buf, 1, BUF_SIZE, fp);
                if (read_cnt < BUF_SIZE)
                {
                    write(clnt_sock, buf, read_cnt);
                    break;
                }
                write(clnt_sock, buf, BUF_SIZE);
            }
        }
        fclose(fp);


        printf("가위바위보 시작\n");
        fp=fopen("option.txt","ab");
        write(clnt_sock, informsg, sizeof(informsg));
        memset(buffer, 0, sizeof(buffer));
        while ((str_len = read(clnt_sock, buffer, BUF_SIZE)) != 0)
        {

            if (strncmp(buffer, "quit", 3) == 0)
            {
                puts("유저와의 연결을 종료합니다.");
                close(clnt_sock);
                fclose(fp);
                exit(0);
            }
            if (strncmp(buffer, "0", 1))
            {
                value = 0;//draw
            }
            if (strncmp(buffer, "1", 1))
            {
                value = 1;//clnt lose
            }
            if (strncmp(buffer, "2", 1))
            {
                value = 2;//clnt win
            }
            srand(time(NULL));
            random = rand() % 3;
            com = who_win(random, value);
            if (com == 0){
                write(clnt_sock, tie, sizeof(tie));
                fwrite(zero,sizeof(zero),1,fp);
            }
            else if (com == 1){
                write(clnt_sock, lose, sizeof(lose));
                fwrite(one,sizeof(one),1,fp);
            }
            else if (com == -1){
                write(clnt_sock, win, sizeof(win));
                fwrite(two,sizeof(two),1,fp);
            }
        }
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
        return 0; //비김
    }
    else if ((a == 0 && b == 2) || (a == 1 && b == 0) || (a == 2 && b == 1)) //클라가 짐
    {
        return 1;
    }
    else
    {
        return -1; //클라가 이김
    }
}
