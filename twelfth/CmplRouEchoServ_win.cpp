//#include <stdio.h>
//#include <stdlib.h>
//#include <WinSock2.h>
//
//#pragma comment(lib, "ws2_32.lib")
//
//#define BUF_SIZE 1024
//
//void CALLBACK ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
//void CALLBACK WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
//void ErrorHandling(char* message);
//
//typedef struct {
//	SOCKET hClntSock;
//	char buf[BUF_SIZE];
//	WSABUF wsabuf;
//} PER_IO_DATA, *LPPER_IO_DATA;
//
//int main(int argc, char* argv[]) {
//	WSADATA wsaData;
//	SOCKET hListenSock, hRecvSock;
//	SOCKADDR_IN lisnAddr, recvAddr;
//	LPWSAOVERLAPPED lpOvlp;
//	DWORD recvBytes;
//	LPPER_IO_DATA hbInfo;
//	int recvAddrSize;
//	DWORD flagInfo = 0;
//
//	//if (argc != 2) {
//	//	printf("Usage: %s <port> \n", argv[0]);
//	//	exit(1);
//	//}
//	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
//		ErrorHandling("WSAStartup() error");
//
//	//hListenSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//	u_long mode = 1;
//	hListenSock = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
//	ioctlsocket(hListenSock, FIONBIO, &mode);
//
//	memset(&lisnAddr, 0, sizeof(lisnAddr));
//	lisnAddr.sin_family = AF_INET;
//	lisnAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//	lisnAddr.sin_port = htons(atoi("9999"));
//	
//	if (bind(hListenSock, (SOCKADDR*)&lisnAddr, sizeof(lisnAddr)) == SOCKET_ERROR)
//		ErrorHandling("bind() error");
//	if (listen(hListenSock, 5) == SOCKET_ERROR)
//		ErrorHandling("listen() error");
//
//	recvAddrSize = sizeof(recvAddr);
//	while (1) {
//		SleepEx(100, TRUE);	//for alertable wait state
//		hRecvSock = accept(hListenSock, (SOCKADDR*)&recvAddr, &recvAddrSize);
//		if (hRecvSock == INVALID_SOCKET) {
//			if (WSAGetLastError() == WSAEWOULDBLOCK)	continue;
//			else
//				ErrorHandling("accept() error");
//		}
//		puts("Client connected......");
//
//		lpOvlp = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
//		memset(lpOvlp, 0, sizeof(WSAOVERLAPPED));
//
//		hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
//		hbInfo->hClntSock = (DWORD)hRecvSock;
//		(hbInfo->wsabuf).buf = hbInfo->buf;
//		(hbInfo->wsabuf).len = BUF_SIZE;
//
//		lpOvlp->hEvent = (HANDLE)hbInfo;
//		WSARecv(hRecvSock, &(hbInfo->wsabuf), 1, &recvBytes, &flagInfo, lpOvlp, ReadCompRoutine);
//	}
//	closesocket(hRecvSock);
//	closesocket(hListenSock);
//	WSACleanup();
//	system("pause");
//	return 0;
//}
//
//void CALLBACK ReadCompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags) {
//	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
//	SOCKET hSock = hbInfo->hClntSock;
//	LPWSABUF bufInfo = &(hbInfo->wsabuf);
//	DWORD sentBytes;
//
//	if (szRecvBytes == 0) {
//		closesocket(hSock);
//		free(lpOverlapped->hEvent);
//		free(lpOverlapped);
//		puts("Client disconnected...");
//	}
//	else {
//		bufInfo->len = szRecvBytes;
//		WSASend(hSock, bufInfo, 1, &sentBytes, 0, lpOverlapped, WriteCompRoutine);
//	}
//}
//
//void CALLBACK WriteCompRoutine(DWORD dwError, DWORD szSendBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags) {
//	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
//	SOCKET hSock = hbInfo->hClntSock;
//	LPWSABUF bufInfo = &(hbInfo->wsabuf);
//	DWORD recvBytes;
//	DWORD flagInfo = 0;
//	WSARecv(hSock, bufInfo, 1, &recvBytes, &flagInfo, lpOverlapped, ReadCompRoutine);
//}
//
//void ErrorHandling(char* message) {
//	fputs(message, stderr);
//	fputc('\n', stderr);
//	exit(1);
//}
