/*************************************************************************
	> File Name: pthread_mutex.c
	> Author: Tokey
	> Mail: TokeyRoad@163.com 
	> Created Time: 2020年06月04日 星期四 14时49分04秒
 ************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <errno.h>

int main(){
	pthread_mutex_t mymutex;
	pthread_mutex_init(&mymutex, NULL);
	int ret = pthread_mutex_lock(&mymutex);
	if(ret != 0){
		if(errno == EBUSY){
			printf("EBUSY\n");
		}
		printf("Failed to destory mutex.\n");
	}

	ret = pthread_mutex_unlock(&mymutex);
	ret = pthread_mutex_destroy(&mymutex);
	if(ret == 0){
		printf("Succeed to destroy mutex.\n");
	}
	
	return 0;
}


