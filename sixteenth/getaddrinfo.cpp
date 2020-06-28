/*************************************************************************
	> File Name: getaddrinfo.cpp
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年06月28日 星期日 16时13分50秒
 ************************************************************************/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
//#include <arpa/inet.h>


/*
 * int getaddrinfo(const char* node,
 *					const char* service,
 *					const struct addrinfo* hints,
 *					struct addrinfo** res);
 */

/*
 *	struct addrinfo{
 *		int				ai_flags;
 *		int				ai_family;
 *		int				ai_socktype;
 *		int				ai_protocol;
 *		socklen_t		ai_addrlen;
 *		struct sockaddr* ai_addr;
 *		char*			ai_canonname;
 *		struct addrinfo* ai_next;
 *	}
 */

/*	void freeaddrinfo(struct addrinfo* res);	*/
int main(){
	struct addrinfo hints = { 0 };
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;	//family
	hints.ai_socktype = SOCK_STREAM;	//socktype
	struct addrinfo* res;
	const char* host = "baidu.com";
	char port[] = "3000";
	int n = getaddrinfo(host, port, &hints, &res);
	if(n == 0){
		//success
		std::cout << res->ai_addr << std::endl;
		
		freeaddrinfo(res);
	}
	return 0;
}
