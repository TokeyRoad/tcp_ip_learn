/*************************************************************************
	> File Name: send_recv.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月18日 星期六 15时40分55秒
 ************************************************************************/

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>


/**************oob_send_recv******************/
/*client*/
write(sock, "123", strlen("123"));
send(sock, "4", strlen("4"), MSG_OOB);
write(sock, "567", strlen("567"));
send(sock, "890", strlen("890"),MSG_OOB);

/*server*/
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
void urg_handler(int sig);
struct sigaction act;
act.sa_handler = urg_handler;
fcntl(recv_sock, F_SETOWN, getpid());
state = sigaction(SIGURG, &act, 0);
while((str_len = recv(recv_sock, buf, sizeof(buf), 0)) != 0){
	//todo something
}

/*
 * fcntl 函数：将 文件描述符recv_sock指向的套接字引发的SIGURG信号
 *			   处理进程变为getpid()返回值作为ID的进程（即当前进程）
 * */
/* 结论
 * 通过MSG_OOB可选项传递数据时不会加快数据传输速度，而且信号处理时
 * 读取数据也只能读取1个字节，剩余数据只能通过未设置MSG_OOB可选项
 * 的普通输入函数读取。
 * MSG_OOB 的真正意义在于督促数据接收对象尽快处理数据。
 * TCP 保持传输顺序的传输特性依然成立。
 * */


/**************MSG_PEEK******************/
/*server*/

while(1){
	str_len = recv(recv_sock, buf, sizeof(buf) - 1, MSG_PEEK|MSG_DONTWAIT);
	if(str_lem > 0)	break;
}
buf[str_len] = 0;
printf("read first : %s \n", buf);
str_len = recv(recv_sock, buf, sizeof(buf) - 1, 0);
buf[str_len] = 0;
printf("read second : %s \n", buf);

/*
 * 调用recv函数同时传入MSG_PEEK，保证即使没有数据，recv函数也不会进入阻塞
 * 假如有数据，读取之后缓存区的数据不会被删除。
 * 再次调用recv函数，未设置任何可选项。缓存区数据被删除。
 * MSG_PEEK一般和MSG_DONTWAIT同时使用
 * */


/**************readv_writev******************/
#include <sys/uio.h>
/*
 * struct iovec{
 *		void* iov_base;
 *		size_t iov_len;
 * }
 * */

/*writev*/
struct iovec vec[2];
char buf1[] = "abcdefg";
char buf2[] = "1234567";
int str_len = 0;
vec[0].iov_base = buf1;
vec[0].iov_len = 3;
vec[1].iov_base = buf2;
vec[1].iov_len = 4;

str_len = writev(1, vec, 2);
/*
 * 第一个参数 套接字或者文件描述符
 * 第二个参数 iovec结构体数组的地址
 * 第三个参数 第二个参数中数组的长度
 * */

/*readv*/
#define BUF_SIZE 30
struct iovec vec[2];
char buf1[BUF_SIZE] = {0,};
char buf2[BUF_SIZE] = {0,};
int str_len = 0;

vec[0].iov_base = buf1;
vec[0].iov_len = 5;
vec[1].iov_base = buf2;
vec[1].iov_len = BUF_SIZE;
/*
 * vec[0]中 最多只能 保存5个字节的数据
 */

str_len = readv(0, vec, 2);
/*
 * 第一个参数 套接字或者文件描述符
 * 第二个参数 用于接收数据的iovec结构体数组
 * 第三个参数 数组长度
 */




