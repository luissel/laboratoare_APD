#include<string.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<pthread.h>

/**
 * @author cristian.chilipirea
 * 
 */

int a;
int p = 4;
int n = 1024*1024*100;

void *threadFunction(void *args){
    int thread_id = *(int *)args;
    int c;
	int start = n / p * thread_id;
	int stop = n / p * (thread_id + 1);

	if (thread_id == p - 1) {
		stop = n;
	}
	
	for (int i = start; i < stop; ++i) {
		c = sqrt(a);
	}
}

int main(int argc, char * argv[]) {
	int i, j;
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