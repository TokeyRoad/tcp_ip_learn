/*************************************************************************
	> File Name: epoll_et.cpp
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年06月22日 星期一 10时18分27秒
 ************************************************************************/

#include <iostream>
#include <stdio.h>
#include <string>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define EPOLL_SIZE 50
#define BUF_SIZE 100


int main(){
	int epfd, listenfd;
	struct sockaddr_in serAddr, clientAddr;
	char buf[BUF_SIZE] = { 0 };

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd == -1) {
		std::cout << "socket create error" << std::endl;
		return -1;
	}

	serAddr.sin_port = htons(3000);
	serAddr.sin_family = AF_INET;
	serAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int oldflag = fcntl(listenfd, F_GETFL, 0);
	int newflag = oldflag | O_NONBLOCK;
	if(fcntl(listenfd, F_SETFL, newflag) == -1){
		std::cout << "listenfd set nonblock error" << std::endl;
		close(listenfd);
		return -1;
	}

	if(bind(listenfd, (struct sockaddr*)&serAddr, sizeof(serAddr)) == -1){
		std::cout << "bind error" << std::endl;
		close(listenfd);
		return -1;
	}

	if(listen(listenfd, SOMAXCONN) == -1){
		std::cout << "listen error" << std::endl;
		close(listenfd);
		return -1;
	}

	epfd = epoll_create(EPOLL_SIZE);
	struct epoll_event listen_fd_event;
	listen_fd_event.data.fd = listenfd;
	listen_fd_event.events = EPOLLIN;
	
	listen_fd_event.events |= EPOLLET;
	
	if(epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &listen_fd_event) == -1){
		std::cout << "epoll_ctl add listenfd error" << std::endl;
		close(listenfd);
		return -1;
	}

	int index = 0;
	int n = 0;
	while(true){
		struct epoll_event ep_events[EPOLL_SIZE];
		n = epoll_wait(epfd, ep_events, EPOLL_SIZE, 1000);
		if(n < 0){
			if(errno == EINTR){
				continue;
			}
			break;
		}
		else if(n == 0){
			continue;
		}

		for(size_t i = 0; i < n; ++i){
			if(ep_events[i].events & EPOLLIN){
				if(ep_events[i].data.fd == listenfd){
					//accept
					socklen_t clientlen = sizeof(clientAddr);
					int clientfd = accept(listenfd, (struct sockaddr*)&clientAddr, &clientlen);
					if(clientfd == -1){
						std::cout << "clientfd accept error" << std::endl;
						continue;
					}
					epoll_event clientfd_event;
					clientfd_event.data.fd = clientfd;
					clientfd_event.events = EPOLLIN;
					//set ET or LT  default LT
					clientfd_event.events |= EPOLLET;

					int oldtemp = fcntl(clientfd, F_GETFL, 0);
					int newtemp = oldtemp | O_NONBLOCK;
					if(fcntl(clientfd, F_SETFL, newtemp) == -1){
						std::cout << "clientfd set nonblock error" << std::endl;
						close(clientfd);
						continue;
					}

					if(epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &clientfd_event) == -1){
						std::cout << "epoll_ctl add clientfd error" << std::endl;
						close(clientfd);
						continue;
					}
				}
				else{
					bool exit = false;
					while(true){
						char buff[256];
						int nRecv = ::recv(ep_events[i].data.fd, buff, 1, 0);
						if(nRecv == -1){
							if(errno == EWOULDBLOCK){
								std::cout << "clientfd" << ep_events[i].data.fd << "recv over" << std::endl;
								break;
							}
							else if(errno == EINTR){
								continue;
							}
							exit = true;
							break;
						}
						else if(nRecv == 0){
							exit =true;
							break;
						}
						else{
							std::cout << "recv from clientfd" << ep_events[i].data.fd << ":" << buff << std::endl;
						}
					}
					if(exit){
						if(epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL) != -1){
							std::cout << "client disconnected, clientfd:" << ep_events[i].data.fd << std::endl;
						}
						else{
							std::cout << "client remove error, clientfd:" << ep_events[i].data.fd << std::endl;
						}
						close(ep_events[i].data.fd);
					}
					/*
					char buff[256];
					int nRecv = ::recv(ep_events[i].data.fd, buff, 1, 0);
					if(nRecv <= 0){
						if(errno == EINTR){
							continue;
						}
						if(epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL) != -1){
							std::cout << "client disconnected, clientfd:" << ep_events[i].data.fd << std::endl;
						}
						close(ep_events[i].data.fd);
					}
					else{
						std::cout << "recv from clientfd" << ep_events[i].data.fd << ":" << buff << std::endl;
					}
					*/
				}
			}
			else if(ep_events[i].events & EPOLLOUT){
				//TODO del
			}
			else if(ep_events[i].events & EPOLLERR){
				//TODO err
			}
		}
	}
	close(listenfd);
	return 0;
}


