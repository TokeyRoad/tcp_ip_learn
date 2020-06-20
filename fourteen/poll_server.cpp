/*************************************************************************
	> File Name: poll_server.cpp
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年06月19日 星期五 15时24分48秒
 ************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define INVALID_FD -1

int main(){
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1){
		std::cout << "listen socket create error" << std::endl;
		return -1;
	}
	
	//set nonblock
	int flag = fcntl(listenfd, F_GETFL, 0);
	int newFlag = flag | O_NONBLOCK;
	if(fcntl(listenfd, F_SETFL, newFlag) == -1){
		std::cout << "set nonblock error" << std::endl;
		close(listenfd);
		return -1;
	}

	//set reuse
	int on = 1;
	::setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on));
	::setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, (char*)&on, sizeof(on));

	//init sockaddr
	struct sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(3000);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr(INADDR_ANY)
	if(bind(listenfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1){
		std::cout << "bind error" << std::endl;
		close(listenfd);
		return -1;
	}

	if(listen(listenfd, SOMAXCONN) == -1){
		std::cout << "listen error" << std::endl;
		close(listenfd);
		return -1;
	}

	//init poll
	std::vector<pollfd> fds;
	pollfd listen_fd_info;
	listen_fd_info.fd = listenfd;
	listen_fd_info.events = POLLIN;
	listen_fd_info.revents = 0;
	fds.push_back(listen_fd_info);

	bool exist_invalid_fd;
	int n;
	while(true){
		exist_invalid_fd = false;
		n = poll(&fds[0], fds.size(), 1000);
		if(n < 0){
			if(errno == EINTR){
				continue;
			}else{
				std::cout << "poll error" << errno << std::endl;
				break;
			}
		}
		else if(n == 0){
			//timeout
			continue;
		}

		for(size_t i = 0; i < fds.size(); ++i){
			if(fds[i].revents & POLLIN){
				if(fds[i].fd == listenfd){
					//accept
					struct sockaddr_in clientaddr;
					socklen_t clientlen = sizeof(clientaddr);
					int clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, &clientlen);
					if(clientfd != -1){
						//set nonblock
						int nonflag = fcntl(clientfd, F_GETFL, 0);
						int newnonflag = nonflag | O_NONBLOCK;
						if(fcntl(clientfd, F_SETFL, newnonflag) == -1){
							close(clientfd);
							std::cout << "clientfd set nonblock error" << std::endl;
						}
						else{
							struct pollfd client_fd_info;
							client_fd_info.fd = clientfd;
							client_fd_info.events = POLLIN;
							client_fd_info.revents = 0;
							fds.push_back(client_fd_info);
							std::cout << "new client accepted, clientfd:" << clientfd << std::endl;
						}
					}
				}
				else{
					//read
					char buf[64] = { 0 };
					int m = recv(fds[i].fd, buf, sizeof(buf), 0);
					if(m <= 0){
						if(errno != EINTR && errno != EWOULDBLOCK){
							for(auto iter = fds.begin(); iter != fds.end(); ++iter){
								if(iter->fd == fds[i].fd){
									std::cout << "client disconnected, clientfd:" << iter->fd << std::endl;
									close(fds[i].fd);
									iter->fd = INVALID_FD;
									exist_invalid_fd = true;
									break;
								}
							}
						}
					}
						else{
							std::cout << "recv from clientfd<" << fds[i].fd << ">:" << buf << std::endl;
						}
				}
			}
			else if(fds[i].revents & POLLERR){
				//TODO
			}
		}

		if(exist_invalid_fd){
			//clear invalid fd
			for(auto iter = fds.begin(); iter != fds.end();){
				if(iter->fd == INVALID_FD){
					fds.erase(iter);
				}else{
					++iter;
				}
			}
		}
	}
	
	for(auto iter = fds.begin(); iter != fds.end(); ++iter){
		close(iter->fd);
	}

	return 0;
}
