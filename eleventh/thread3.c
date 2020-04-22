/*************************************************************************
	> File Name: thread3.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月21日 星期二 11时48分31秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* calc_thread(void* arg);
int sum = 0;

int main(int argc, char* argv[]){
	pthread_t t1_id, t2_id;
	int t1_ret, t2_ret;
	int arg1 = 1, arg2 = 6;
	pthread_create(&t1_id, NULL, calc_thread, (void*)&arg1);
	pthread_create(&t2_id, NULL, calc_thread, (void*)&arg2);

	if(pthread_join(t1_id, NULL) != 0){
		puts("join() error");
		return -1;
	}
	if(pthread_join(t2_id, NULL) != 0){
		puts("join() error");
		return -1;
	}
	printf("sum:%d \n", sum );
	return 1;
}

void* calc_thread(void* arg){
	int i;
	int st = *((int*)arg);
	for(i = 0; i < 5; ++i){
		sum += st + i;
	}
	return NULL;
}


