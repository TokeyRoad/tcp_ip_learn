/*************************************************************************
	> File Name: thread4.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年04月21日 星期二 14时51分32秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100
void* thread_inc(void* arg);
void* thread_des(void* arg);
long long sum = 0;
pthread_mutex_t mutex;

int main(int argc, char*argv[]){
	pthread_t thread_id[NUM_THREAD];
	int i;
	printf("sizeof long long : %d \n", sizeof(long long));
	pthread_mutex_init(&mutex, NULL);
	for(i = 0; i < NUM_THREAD; ++i){
		if(i % 2){
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		}else{
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
		}
	}
	for(i = 0; i < NUM_THREAD; ++i){
		pthread_join(thread_id[i], NULL);
	}
	printf("result:%lld \n", sum);
	pthread_mutex_destroy(&mutex);
	return 0;
}

void* thread_inc(void* arg){
	int i;
	for(i = 0; i < 5000000; ++i){
		pthread_mutex_lock(&mutex);
		sum+=1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}

void* thread_des(void* arg){
	int i;
	for(i = 0; i < 5000000; ++i){
		pthread_mutex_lock(&mutex);
		sum-=1;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}


