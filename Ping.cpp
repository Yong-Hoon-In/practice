#include "헤더.h"
int main(int argc, char *argv[]) {
	int retval;
	if (argc < 2) {
		printf("Usage:%s <host namen",argv[0]);
		return 1;
	}
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)return 1;
	SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (sock == INVALID_SOCKET)err_quit("socket()");
	int optval = 1000;
	retval = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)err_quit("setsockopt()");

	retval = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char *)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR)err_quit("setsockopt()");
	SOCKADDR_IN destaddr;
	ZeroMemory(&destaddr, sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	IN_ADDR addr;
	if (GetIPAdder(argv[1], &addr))
		destaddr.sin_addr = addr;
	else
		return 1;
	ICMPMESSAGE icmpmsg;
	char buf[BUFSIZE];
	SOCKADDR_IN peeraddr;
	int addrlen;
	for (int i = 0; i < 4; i++) {
		ZeroMemory(&icmpmsg, sizeof(icmpmsg));
		icmpmsg.icmp_type = ICMP_ECHOREQUEST;
		icmpmsg.icmp_code = 0;
		icmpmsg.icmp_id = (u_short)GetCurrentProcessId();
		icmpmsg.icmp_seq = i;
		icmpmsg.icmp_cksum = checksum((u_short*)&icmpmsg, sizeof(icmpmsg));
		retval = sendto(sock, (char*)&icmpmsg, sizeof(icmpmsg), 0, (SOCKADDR*)&destaddr, sizeof(destaddr));
		if (retval == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT) {
				printf("[오류]Send timed out!\n");
				continue;
			}
			err_display("sendto");
			break;
		}
		addrlen = sizeof(peeraddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR*)&peeraddr, &addrlen);
		if (retval == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAETIMEDOUT) {
				printf("오류 Requesttimeout\n");
				continue;
			}
			err_display("recvfrom()");
			break;
		}
		DecodeICMPMessage(buf, retval, &peeraddr);
		Sleep(1111);
	}
	closesocket(sock);
	WSACleanup();
	return 0;
}
void DecodeICMPMessage(char *buf, int bytes, SOCKADDR_IN *from) {
	IPHEADER *iphdr = (IPHEADER*)buf;
	int iphdrlen = iphdr->ip_hl * 4;
	ICMPMESSAGE *icmpmsg = (ICMPMESSAGE *)(buf + iphdrlen);
	if (bytes - iphdrlen < 8) {
		printf("오류 ICMP packet is too short\n");
			return;
	}
	if (icmpmsg->icmp_id != (u_short)GetCurrentProcessId()) {
		printf("오류 Not a response to echo request\n");
			return;
	}
//	if (icmpmsg->icmp_type != ICMP_ECHOREPLY)) {
//		printf("오류 not echo reply packet\n");
//		return;
//	}
	printf("reply from %s:total bytes= %d, seq = %d\n", inet_ntoa(from->sin_addr), bytes, icmpmsg->icmp_seq);
	return;
}
BOOL GetIPAdder(char *name, IN_ADDR *addr) {
	HOSTENT *ptr = gethostbyname(name);
	if (ptr == nullptr) {
		err_display("gethostbyname");
		return FALSE;
	}
	if (ptr->h_addrtype != AF_INET)
		return FALSE;
	memcpy(addr, ptr->h_addr_list, ptr->h_length);
		return TRUE;
}
u_short checksum(u_short *buffer, int size) {
	u_long cksum = 0;
	u_short *ptr = buffer;
	int left = size;
	while (left > 1) {
		cksum += *ptr++;
		left -= sizeof(u_short);
	}
	if (left == 1)
		cksum += *(u_char*)buffer;
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (u_short)(~cksum);


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

//헤더.h
#pragma once
#pragma warning (disable :4996)
#pragma comment(lib,"ws2_32")
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#define BUFSIZE 1500
typedef struct _IPHEADER
{
	u_char ip_hl : 4;
	u_char ip_v : 4;
	u_char ip_tos;
	short ip_len;
	u_short ip_id;
	short ip_off;
	u_char ip_ttl;
	u_char ip_p;
	u_short ip_cksum;
	IN_ADDR ip_src;
	IN_ADDR ip_dst;
} IPHEADER;

typedef struct _ICMPMESSAGE {
	u_char icmp_type;
	u_char icmp_code;
	u_short icmp_cksum;
	u_short icmp_id;
	u_short icmp_seq;
}ICMPMESSAGE;
#define ICMP_ECHOREQUEST 8;
#define ICMP_ECHOREPLY 8;
void DecodeICMPMessage(char *buf, int bytes, SOCKADDR_IN *from);
BOOL GetIPAdder(char *name, IN_ADDR *addr);
u_short checksum(u_short *buffer, int size);
void err_quit(const char *msg);
void err_display(const char *msg);
