#pragma comment(lib,"ws2_32")
#pragma warning(disable : 4996)
#include <WinSock2.h>
#include <ws2bth.h>
#include <initguid.h>
#include <stdlib.h>
#include <stdio.h>
#define SERVERADDR "00:09:31:00:13:F9"
#define BUFSIZE 512
DEFINE_GUID(BthServer_Service, 0x4672de25, 0x588d, 0x48af, 0x80, 0x73, 0x5f, 0x2b, 0x7b, 0x0, 0x60, 0x1f);
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
int main(int argc, char *argv[]) {
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	DWORD qslen = sizeof(WSAQUERYSET);
	WSAQUERYSET *qs = (WSAQUERYSET*)malloc(qslen);
	ZeroMemory(qs, qslen);
	qs->dwNameSpace = NS_BTH;
	qs->lpServiceClassId = (GUID*)&BthServer_Service;
	DWORD flags = LUP_FLUSHCACHE | LUP_RETURN_ADDR;

	HANDLE hLookup;
	retval = WSALookupServiceBegin(qs, flags, &hLookup);
	if (retval == SOCKET_ERROR) {
		printf("[오류] 발견된 블루투스 장치 없음\n");
		exit(1);
	}
	SOCKADDR_BTH *sa = NULL;
	int serverport = 0;
	bool done = false;
	while (!done) {
		retval = WSALookupServiceNext(hLookup, flags, &qslen, qs);
		if (retval == NO_ERROR) {
			sa = (SOCKADDR_BTH*)qs->lpcsaBuffer->RemoteAddr.lpSockaddr;
			serverport = sa->port;
			break;
		}
		else {
			if (WSAGetLastError() == WSAEFAULT) {
				free(qs);
				qs = (WSAQUERYSET*)malloc(qslen);
			}
			else {
				done = true;
			}
		}
	}
	if (sa == NULL) {
		printf("[오류]블루투스 장치(%s)에서 실행중인 서버 없음\n", SERVERADDR);
		exit(1);
	}
	WSALookupServiceEnd(hLookup);
	free(qs);
	SOCKET sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	//bind
	SOCKADDR_BTH serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	int addrlen = sizeof(serveraddr);
	WSAStringToAddress((char*)SERVERADDR, AF_BTH, NULL, (SOCKADDR*)&serveraddr, &addrlen);
	serveraddr.port = serverport;
	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	char buf[BUFSIZE + 1];
	int len;
	while (1) {
		printf("\n보낼데이터");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL)break;
		len = strlen(buf);
		if (buf[len - 1] == '\n')buf[len - 1] = '\0';
		if (strlen(buf) == 0)break;
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		printf("[블루투스 클라이언트] %d 바이트를 보냈습니다\n", retval);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}
