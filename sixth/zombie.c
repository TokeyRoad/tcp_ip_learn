/*************************************************************************
	> File Name: zombie.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月17日 星期五 14时15分06秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]){
	pid_t pid = fork();

	if(pid == 0){
		puts("Hi, I am a child process");
	}else{
		printf("Child process ID: %d \n", pid);
		sleep(60);
	}

	if(pid == 0){
		puts("End child process");
	}else{
		puts("End parent process");
	}
	return 0;
}

