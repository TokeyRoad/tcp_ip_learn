/*************************************************************************
	> File Name: inet_aton_ntoa.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月14日 星期二 15时03分55秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>

void error_handling(char* message);

int main(int argc, char* argv[]){
	printf("*************inet_ntoa***************\n");
	/* inet_ntoa */
	struct sockaddr_in addr1,addr2;
	char* str_ptr;
	char str_arr[20];

	addr1.sin_addr.s_addr = htonl(0x1020304);
	addr2.sin_addr.s_addr = htonl(0x1010101);

	str_ptr = inet_ntoa(addr1.sin_addr);
	strcpy(str_arr, str_ptr);
	printf("Dotted-Decimal notation1:%s \n", str_ptr);

	inet_ntoa(addr2.sin_addr);
	printf("Dotted-Decimal notation2: %s \n", str_ptr);
	printf("Dotted-Decimal notation3: %s \n", str_arr);

	printf("*************inet_ntoa***************\n");
	/* inet_aton */
	char* addr = "127.232.124.79";
	struct sockaddr_in addr_inet;

	if(!inet_aton(addr, &addr_inet.sin_addr))
		error_handling("Conversion error");
	else
		printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);

	return 0;
}

void error_handling(char* message){
	fputs(message,stderr);
	fputc('\n', stderr);
	exit(1);
}


