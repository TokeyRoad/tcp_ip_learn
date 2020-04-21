/*************************************************************************
	> File Name: echo_epollserv.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月20日 星期一 16时40分07秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *buf);

/*
 * struct epoll_event{
 *		__uint32_t events;
 *		epoll_data_t data;
 * }
 * 
 * tydedef union epoll_data{
 *		void* ptr;
 *		int fd;
 *		__uint32_t u32;
 *		__uint64_t u64;
 * } epoll_data_t;
 */



int main(int argc, char* argv[]){
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr, clnt_addr;
	socklen_t clnt_addr_size;
	int str_len, i;
	char buf[BUF_SIZE];
	
	struct epoll_event* ep_events;
	struct epoll_event event;
	int epfd, event_clnt;
	if(argc != 2){
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
		error_handling("bind() error");
	}
	if(listen(serv_sock, 5) == -1){
		error_handling("listen() error");
	}

	epfd = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

	event.events = EPOLLIN;
	event.data.fd = serv_sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);

	while(1){
		event_clnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
		if(event_clnt == -1){
			puts("epoll_wait() error");
			break;
		}
		for(i = 0; i < event_clnt; ++i){
			if(ep_events[i].data.fd == serv_sock){
				clnt_addr_size = sizeof(clnt_addr);
				clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
				event.events = EPOLLIN;
				event.data.fd = clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);
				printf("connected client:%d \n", clnt_sock);
			}
			else{
				str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
				if(str_len == 0){
					epoll_ctl(
						epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client: %d \n", ep_events[i].data.fd);
				}
				else{
					write(ep_events[i].data.fd, buf, str_len);
				}
			}
		}
	}
	close(serv_sock);
	close(epfd);
	return 0;
}

void error_handling(char* buf){
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}

