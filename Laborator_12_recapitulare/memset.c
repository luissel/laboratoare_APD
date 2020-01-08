#include<string.h>
#include<stdio.h>
#include<time.h>
#include<pthread.h>

/**
 * @author cristian.chilipirea
 * 
 */

int n1GB = 1024*1024*1024;
char v[1024*1024*1024];
int p = 6;


void *threadFunction(void *args){
	int n = 32;
    int thread_id = *(int *)args;
	int start = n / p * thread_id;
	int stop = n / p * (thread_id + 1);

	if (thread_id == p - 1) {
		stop = n;
	}

	for (int i = start; i < stop; ++i) {
		memset(v + (i * 32 * 1024 * 1024), '1', 32 * 1024 * 1024);
	}
}
 
int main(int argc, char * argv[]) {
	int c;
	memset(v, '1', n1GB);
	clock_t begin = clock();

	pthread_t tid[p];
    int thread_id[p];

	for(int i = 0; i < p; ++i){
        thread_id[i] = i;
    }

	for (int i = 0; i < p; ++i) {
		pthread_create(&tid[i], NULL, threadFunction, &thread_id[i]);
	}

	for (int i = 0; i < p; ++i) {
		 pthread_join(tid[i], NULL);
	}

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("%f\n",time_spent);
	return 0;
}