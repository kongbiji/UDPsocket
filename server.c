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
	SOCKET serverSocket;
	SOCKADDR_IN serverAddr;
	SOCKADDR_IN clientAddr;

	char buf[BUFSIZE];
	int recvLen;
	int sendLen;
	int clientSize;

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

	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (serverSocket == INVALID_SOCKET) {
		printf("[-] Socket creation failed.\n");
		WSACleanup();
		return;
	}
	printf("[+] Socket create.\n");

	if (bind(serverSocket, (struct sockaddr*) & serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		printf("[-] Bind failed.\n");
		closesocket(serverSocket);
		WSACleanup();
		return;
	}
	printf("[+] Bind success.\n");

	int i = 1;

	while (1) {
		clientSize = sizeof(clientAddr);
		recvLen = recvfrom(serverSocket, buf, BUFSIZE, 0, 
			(struct sockaddr*) & clientAddr, &clientSize);
		if (recvLen < 0) {
			printf("[-] recvfrom() error.\n");
			return;
		}
		printf("%d: %s\n", i ,buf);
		sendLen = sendto(serverSocket, buf, recvLen, 0,
			(struct sockaddr *)&clientAddr, sizeof(clientAddr));
		if (sendLen != recvLen) {
			printf("[-] sendto() error.\n");
			return;
		}
		i++;
	}
	closesocket(serverSocket);
	WSACleanup();
}
