#pragma warning(disable : 4996)
#pragma comment(lib,"ws2_32")
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#define SERVERPORT 9000
#define BUFSIZE 512
struct SOCKETINFO {
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	BOOL recvdelayed;
};

int nTotalSockets = 0;
SOCKETINFO *SocketInfoArray[WSA_MAXIMUM_WAIT_EVENTS];
WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];

BOOL AddSocketInfo(SOCKET sock);
void RemoveSocketInfo(int index);
void err_quit(const char *msg);
void err_display(const char *msg);
void err_display(int errcode);

int main(int argc, char *argv[]) {
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	//socket
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	//listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)err_quit("listen()");
	AddSocketInfo(listen_sock);
	retval = WSAEventSelect(listen_sock, EventArray[nTotalSockets - 1], FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR)err_quit("WSAEventselect()");

	WSANETWORKEVENTS NetworkEvent;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int i, addrlen;

	while (1) {
		i = WSAWaitForMultipleEvents(nTotalSockets, EventArray, FALSE, WSA_INFINITE, FALSE);
		if (i == WSA_WAIT_FAILED)continue;
		i -= WSA_WAIT_EVENT_0;
		retval = WSAEnumNetworkEvents(SocketInfoArray[i]->sock, EventArray[i], &NetworkEvent);
		if (retval == SOCKET_ERROR)continue;

		if (NetworkEvent.lNetworkEvents&FD_ACCEPT) {
			if (NetworkEvent.iErrorCode[FD_ACCEPT_BIT] != 0) {
				err_display(NetworkEvent.iErrorCode[FD_ACCEPT_BIT]);
				continue;
			}
			addrlen = sizeof(clientaddr);
			client_sock = accept(SocketInfoArray[i]->sock, (SOCKADDR*)&clientaddr, &addrlen);
			if (client_sock == INVALID_SOCKET) {
				err_display("accept()");
				continue;
			}
			printf("[TCP서버] 클라이언트 접속:IP 주소=%s, 포트번호=%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
			if (nTotalSockets >= WSA_MAXIMUM_WAIT_EVENTS) {
				printf("[오류] 더 이상 접속을 받아들일 수 없습니다\n");
				closesocket(client_sock);
				continue;
			}
			AddSocketInfo(client_sock);
			retval = WSAEventSelect(client_sock, EventArray[nTotalSockets - 1], FD_READ | FD_WRITE | FD_CLOSE);
			if (retval == SOCKET_ERROR)err_quit("WSAEventSelect()");
		}
		if (NetworkEvent.lNetworkEvents&FD_READ || NetworkEvent.lNetworkEvents&FD_WRITE)
		{
			if (NetworkEvent.lNetworkEvents&FD_READ && NetworkEvent.iErrorCode[FD_READ_BIT] != 0) {
				err_display(NetworkEvent.iErrorCode[FD_READ_BIT]);
				continue;
			}
			if (NetworkEvent.lNetworkEvents&FD_WRITE&&NetworkEvent.iErrorCode[FD_WRITE_BIT] != 0) {
				err_display(NetworkEvent.iErrorCode[FD_WRITE_BIT]);
				continue;
			}
			SOCKETINFO* ptr = SocketInfoArray[i];
			if (ptr->recvbytes == 0) {
				retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
				if (retval == SOCKET_ERROR) {
					if (WSAGetLastError() != WSAEWOULDBLOCK) {
						err_display("recv()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				ptr->recvbytes = retval;
				ptr->buf[retval] = '\0';
				addrlen = sizeof(clientaddr);
				getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
				printf("[TCP \%s: %d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);
			}
			if (ptr->recvbytes > ptr->sendbytes) {
				retval = send(ptr->sock, ptr->buf + ptr->sendbytes, ptr->recvbytes - ptr->sendbytes, 0);
				if (retval == SOCKET_ERROR) {
					if (WSAGetLastError() != WSAEWOULDBLOCK) {
						err_display("send()");
						RemoveSocketInfo(i);
					}
					continue;
				}
				ptr->sendbytes += retval;
				if (ptr->recvbytes == ptr->sendbytes)
					ptr->recvbytes = ptr->sendbytes = 0;
			}
		}
		if (NetworkEvent.lNetworkEvents&FD_CLOSE) {
			if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
				err_display(NetworkEvent.iErrorCode[FD_CLOSE_BIT]);
			RemoveSocketInfo(i);
		}
	}
	WSACleanup();
	return 0;
}
BOOL AddSocketInfo(SOCKET sock) {
	SOCKETINFO *ptr = new SOCKETINFO;
	if (ptr == NULL) {
		printf("[오류] 메모리부족\n");
		return FALSE;
	}
	WSAEVENT hEvent = WSACreateEvent();
	if (hEvent == WSA_INVALID_EVENT) {
		err_display("WSACreateEvnet()");
		return FALSE;
	}
	
	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	SocketInfoArray[nTotalSockets] = ptr;
	EventArray[nTotalSockets] = hEvent;
	++nTotalSockets;

	return TRUE;
}
void RemoveSocketInfo(int index) {
	SOCKETINFO *ptr = new SOCKETINFO;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(ptr->sock, (SOCKADDR*)&clientaddr, &addrlen);
	printf("[TCP서버] 클라이언트 종료:IP 주소=%s,포트번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	closesocket(ptr->sock);
	delete ptr;
	WSACloseEvent(EventArray[index]);
	if (index != (nTotalSockets - 1)) {
		SocketInfoArray[index] = SocketInfoArray[nTotalSockets - 1];
		EventArray[index] = EventArray[nTotalSockets - 1];
	}
	--nTotalSockets;
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
void err_display(int errcode) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		errcode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류] %s", (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
