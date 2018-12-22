#pragma warning(disable : 4996)
#pragma comment(lib,"ws2_32")
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#include <stdio.h>
#define MULTICASTIP "235.7.8.9"
#define REMOTEPORT 9000
#define BUFSIZE 512
void err_quit(const char *msg);
void err_display(const char *msg);
int main(int argc, char **argv) {
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	int ttl = 2;
	retval = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&ttl, sizeof(ttl));

	
	SOCKADDR_IN remoteaddr;
	ZeroMemory(&remoteaddr, sizeof(remoteaddr));
	remoteaddr.sin_addr.s_addr = inet_addr(MULTICASTIP);
	remoteaddr.sin_family = AF_INET;
	remoteaddr.sin_port = htons(REMOTEPORT);


	int len;
	char buf[BUFSIZE + 1];
	while (1) {
		//data 받기
		printf("\n[보낼 데이터]");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)
			break;
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			break;
		retval = sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&remoteaddr, sizeof(remoteaddr));
		if (retval == SOCKET_ERROR) {
			err_display("sendto()");
			continue;
		}
		printf("[UDP] %d 바이트를 보냈습니다.\n", retval);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}
void err_quit(const char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
void err_display(const char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
