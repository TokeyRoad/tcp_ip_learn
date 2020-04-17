/*************************************************************************
	> File Name: nagle.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月17日 星期五 11时07分06秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int opt_val = 1;
setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, sizeof(opt_val));

socklen_t opt_len;
optlen = sizeof(opt_val);
int state = getsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &optlen);
if(state){
	printf("getsockopt() error");
}
printf("val: %d \n", opt_val);



