#pragma comment(lib,"ws2_32")
#pragma warning(disable : 4996)
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <ws2bth.h>
#include <initguid.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFSIZE 512
DEFINE_GUID(BthServer_Service, 0x4672de25, 0x588d, 0x48af, 0x80, 0x73, 0x5f, 0x2b, 0x7b, 0x0, 0x60, 0x1f);
void err_quit(const char *msg);
void err_display(const char *msg);
int main(int argc, char *argv[]) {
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	//socket
	SOCKET listen_sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	//bind
	SOCKADDR_BTH serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.addressFamily = AF_BTH;
	serveraddr.btAddr = 0;
	serveraddr.port = BT_PORT_ANY;
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	int addrlen = sizeof(serveraddr);
	retval = getsockname(listen_sock, (SOCKADDR*)&serveraddr, &addrlen);
	if (retval == SOCKET_ERROR)err_quit("bind()");
	printf("[블루투스 서버] 사용중인 포트번호:%d\n", serveraddr.port);

	CSADDR_INFO addrinfo;
	addrinfo.LocalAddr.lpSockaddr = (SOCKADDR*)&serveraddr;
	addrinfo.LocalAddr.iSockaddrLength = sizeof(serveraddr);
	addrinfo.RemoteAddr.lpSockaddr = (SOCKADDR*)&serveraddr;
	addrinfo.RemoteAddr.iSockaddrLength = sizeof(serveraddr);
	addrinfo.iSocketType = SOCK_STREAM;
	addrinfo.iProtocol = BTHPROTO_RFCOMM;
	WSAQUERYSET qset;
	ZeroMemory(&qset, sizeof(qset));
	qset.dwSize = sizeof(qset);
	qset.lpszServiceInstanceName = (LPSTR)"Blutooth Server Test Service";
	qset.lpServiceClassId = (GUID*)&BthServer_Service;
	qset.dwNameSpace = NS_BTH;
	qset.dwNumberOfCsAddrs = 1;
	qset.lpcsaBuffer = &addrinfo;
	retval = WSASetService(&qset, RNRSERVICE_REGISTER, 0);
	if (retval == SOCKET_ERROR)err_quit("WSASetService()");
	retval = listen(listen_sock, 1);
	if (retval == SOCKET_ERROR)err_quit("listen()");
	SOCKET client_sock;
	SOCKADDR_BTH clientaddr; char buf[BUFSIZE + 1];
	while (1) {
		int addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_quit("accept()");
			break;
		}
		printf("[블루투스 서버] 클라이언트 접속\n");
		while (1)
			retval = recv(client_sock, buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;
		buf[retval] = '\0';
		printf("[블루투스 서버] %s\n", buf);
	}
	closesocket(listen_sock);
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
