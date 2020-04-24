#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")

#define BUFSIZE 1024
#define PORT 1234

int main() {
	WSADATA winsockData;
	SOCKET clientSocket;
	SOCKADDR_IN serverAddr;
	SOCKADDR_IN clientAddr;

	char buf[BUFSIZE];
	int recvLen;
	int sendLen;
	int serverSize;

	if (WSAStartup(0x202, &winsockData) == SOCKET_ERROR) {
		printf("[-] Winsock Initialization failed.\n");
		WSACleanup();
		return;
	}
	printf("[+] Winsock Init.\n");
	memset(&serverAddr, 0, sizeof(serverAddr));
	memset(&clientAddr, 0, sizeof(clientAddr));
	memset(buf, 0, BUFSIZE);

	serverAddr.sin_family = AF_INET; // IPv4
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // local test
	serverAddr.sin_port = htons(PORT); // change network by order

	clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSocket == INVALID_SOCKET) {
		printf("[-] Socket creation failed.\n");
		WSACleanup();
		return;
	}
	printf("[+] Socket create.\n");

	while (1) {
		scanf("%s", buf);
		sendLen = sendto(clientSocket, buf, BUFSIZE, 0,
			(struct sockaddr*) & serverAddr, sizeof(serverAddr));
		printf("[+] msg: %s\n", buf);

		if (sendLen != BUFSIZE) {
			printf("[-] sendto() error.\n");
			return;
		}

		serverSize = sizeof(serverAddr);
		memset(buf, 0, BUFSIZE);

		recvLen = recvfrom(clientSocket, buf, BUFSIZE, 0,
			(struct sockaddr*) & serverAddr, &serverSize);
		if (recvLen < 0) {
			printf("[-] recvfrom() error.\n");
			return;
		}
		printf("[+] recv msg: %s\n", buf);
	}
	closesocket(clientSocket);
	WSACleanup();
}
