/*************************************************************************
	> File Name: sigaction.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月17日 星期五 15时22分21秒
 ************************************************************************/

/*
 * struct sigaction{
 *	 void (*sa_handler)(int);
 *	 sigset_t sa_mask;
 *	 int sa_flags;
 * }
 * */


#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
	if(sig == SIGALRM){
		puts("Time out!");
	}
	alarm(2);
}

int main(int argc, char* argv[]){
	int i;
	struct sigaction act;
	act.sa_handler = timeout;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, 0);

	alarm(2);

	for(i = 0; i < 3; ++i){
		puts("wait...");
		sleep(100);
	}
	return 0;
}

