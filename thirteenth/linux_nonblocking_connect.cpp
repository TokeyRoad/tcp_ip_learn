/*************************************************************************
	> File Name: linux_nonblocking_connect.cpp
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年06月19日 星期五 10时23分59秒
 ************************************************************************/

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#define SERVER_PORT		3000
#define SEND_DATA		"Hello World!"
#define SERVER_ADDRESS	"127.0.0.1"

int main(){
	int clientfd = socket(AF_INET, SOCK_STREAM, 0);	//IPPROTO_TCP
	if(clientfd == -1){
		std::cout << "clientfd socket create error!" << std::endl;
		return -1;
	}
	//set nonblocking
	int sockFlag = fcntl(clientfd, F_GETFL, 0);
	int newFlag = sockFlag | O_NONBLOCK;
	int flagRet = fcntl(clientfd, F_SETFL, newFlag);
	if(flagRet == -1){
		close(clientfd);
		std::cout << "set socket to nonblock error" << std::endl;
		return -1;
	}

	//
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
	while(true){
		if(connect(clientfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
			if(errno == EINTR){
				//中断指令
				std::cout << "system interruptted by signal." << std::endl;
				continue;
			}
			else if(errno == EINPROGRESS){
				//连接正在尝试中
				break;
			}
			else{
				//error
				std::cout << "connect error" << std::endl;
				close(clientfd);
				return -1;
			}
		}
		else{
			std::cout << "connect to server successfully." << std::endl;
			close(clientfd);
			return 0;
		}
	}

	fd_set wtSet;
	FD_ZERO(&wtSet);
	FD_SET(clientfd, &wtSet);
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	//if(select(clientfd+1, NULL, &wtSet, NULL, &tv) == 1){
	//	std::cout << "[select] connect to server success." << std::endl;
	//}else{
	//	std::cout << "[select] connect to server error." << std::endl;
	//}
	//
	if(select(clientfd+1, NULL, &wtSet, NULL, &tv) != 1){
		std::cout << "[select] connect to server error" << std::endl;
		close(clientfd);
		return -1;
	}
	int err;
	socklen_t len = static_cast<socklen_t>(sizeof err);
	if(::getsockopt(clientfd, SOL_SOCKET, SO_ERROR, &err, &len) < 0){
		close(clientfd);
		return -1;
	}
	if(err == 0){
		std::cout << "[select] connect to server success." << std::endl;
	}else{
		std::cout << "[select] connect to server error" << std::endl;
	}
	close(clientfd);
	return 0;
}

