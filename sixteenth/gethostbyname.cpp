/*************************************************************************
	> File Name: gethostbyname.cpp
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年06月28日 星期日 15时01分24秒
 ************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

bool connect_to_server(const char* server, short port){
	int hsocket = socket(AF_INET, SOCK_STREAM, 0);
	if(hsocket == -1){
		return false;
	}

	struct sockaddr_in addrSrv = {0};
	struct hostent* pHostent = NULL;
	//addrSrv.sin_addr.s_addr = inet_addr(server);
	if((addrSrv.sin_addr.s_addr = inet_addr(server)) == INADDR_NONE){
		pHostent = gethostbyname(server);
		if(pHostent == NULL){
			return false;
		}
		
		addrSrv.sin_addr.s_addr = *((unsigned long*)pHostent->h_addr_list[0]);
	}
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(port);
	int ret = connect(hsocket, (struct sockaddr*)&addrSrv, sizeof(addrSrv));
	if(ret == -1){
		return false;
	}
	return true;
}

int main(){
	if(connect_to_server("baidu.com", 80)){
		printf("connect success! \n");
	}
	else{
		printf("connect error! \n");
	}
	return 0;
}

