/*************************************************************************
	> File Name: low_open.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月13日 星期一 16时22分14秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char* message);

int main(void){
	int fd;
	char buf[] = "Let's go!\n";

	fd = open("tmp/data.txt", O_CREAT|O_WRONLY|O_TRUNC);
	if(fd == -1){
		error_handling("open()  error!");
	}
	printf("file descriptor: %d \n", fd);

	if(write(fd, buf, sizeof(buf)) == -1){
		error_handling("write() error!");
	}

	close(fd);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

