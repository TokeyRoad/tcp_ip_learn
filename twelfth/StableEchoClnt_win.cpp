#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024

void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;
	char message[BUF_SIZE];
	int strLen, readLen;

	//if (argc != 3) {
	//	printf("Usage: %s <IP> <port> \n", argv[0]);
	//	exit(1);
	//}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error");
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");
	
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	servAddr.sin_port = htons(atoi("9999"));
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error");
	else
		puts("Connected...");

	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		strLen = strlen(message);
		send(hSocket, message, strLen, 0);
		readLen = 0;
		while (1) {
			readLen += recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);
			if(readLen >= strLen)
				break;
		}
		message[strLen] = 0;
		printf("Message from server: %s", message);
	}
	closesocket(hSocket);
	WSACleanup();
	system("pause");
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

