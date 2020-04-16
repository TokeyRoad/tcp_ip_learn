/*************************************************************************
	> File Name: file_client.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月15日 星期三 10时54分31秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc, char* argv[]){
	int serv_sd;
	FILE* fp;
	char buf[BUF_SIZE];
	struct sockaddr_in serv_addr;
	if(argc != 3){
		printf("Usage: %s <IP> <port> \n", argv[0]);
		exit(1);
	}

	fp = fopen("tmp/tmp.txt", "wb");
	serv_sd = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	auto ret = connect(serv_sd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if(ret != 0){
		printf("connect error: %d \n", GetLastError());
	}

	int read_cnt;
	while((read_cnt = read(serv_sd, buf, BUF_SIZE)) > 0){
		printf("read_cnt:%d \n", read_cnt);
		fwrite((void*)buf, 1, read_cnt, fp); 
	}

	puts("Received file data");
	write(serv_sd, "Thank you!", 11);
	fclose(fp);
	close(serv_sd);
	return 0;
}

void error_handling(char* message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


