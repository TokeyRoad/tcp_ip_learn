/*************************************************************************
	> File Name: seventh/pipe1.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: Sat 18 Apr 2020 11:41:31 AM CST
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[]){
	int fds[2];
	char str[] = "Who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);
	pid = fork();
	if(pid == 0){
		write(fds[1], str, sizeof(str));
	}else{
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}
	return 0;
}
