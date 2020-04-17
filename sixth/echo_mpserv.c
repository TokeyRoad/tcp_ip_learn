/*************************************************************************
	> File Name: echo_mpserv.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月17日 星期五 16时29分35秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);
void read_childproc(int sig);

int main(int argc, char* argv[]){
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;
	int str_len, state;
	char buf[BUF_SIZE];
	struct sigaction act;
	pid_t pid;

	if(argc != 2){
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1){
		error_handling("socket() error");
	}

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

	while(1){
		clnt_addr_size = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		if(clnt_sock == -1){
			continue;
		}else{
			printf("new client<%d> connected...\n", clnt_sock);
		}
		pid = fork();
		if(pid==-1){
			close(clnt_sock);
			continue;
		}
		if(pid == 0){	/*子进程运行区域*/
			close(serv_sock);
			while((str_len = read(clnt_sock, buf, BUF_SIZE)) != 0){
				write(clnt_sock, buf, str_len);
			}
			close(clnt_sock);
			printf("client<%d> disconnected.. \n", clnt_sock);
			return 0;
		}else{
			close(clnt_sock);
		}
		printf("next while...\n");
	}
	close(serv_sock);
	return 0;
}

void error_handling(char* message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_childproc(int sig){
	pid_t pid;
	int status;
	pid = waitpid(-1, &status, WNOHANG);
	printf("remove proc id: %d \n", pid);
}


