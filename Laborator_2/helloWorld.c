#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int a;
pthread_barrier_t mybarrier;
pthread_barrier_t mybarrier2;

void* threadFunction(void *var)
{
	int thread_id = *(int*)var;
	
	pthread_barrier_wait(&mybarrier);
	a = 5;
	pthread_barrier_wait(&mybarrier2);
	a += 7;
	
	//printf("Hello world from thread %i and 2\n", thread_id);
}

void* threadFunction2(void *var)
{
	int thread_id = *(int*)var;
	
	a = 3;
	pthread_barrier_wait(&mybarrier);
	pthread_barrier_wait(&mybarrier2);
	a += 2;

	//printf("Hello world from thread %i and 2\n", thread_id);
}

int main(int argc, char **argv){
	int P = 2;
	int i;

	pthread_t tid[P];
	int thread_id[P];

	pthread_barrier_init(&mybarrier, NULL, 2);
	pthread_barrier_init(&mybarrier2, NULL, 2);
	for(i = 0;i < P; i++)
		thread_id[i] = i;


	pthread_create(&(tid[0]), NULL, threadFunction, &(thread_id[0]));
	pthread_create(&(tid[1]), NULL, threadFunction2, &(thread_id[1]));
	

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&mybarrier);
	pthread_barrier_destroy(&mybarrier2);

	printf("a = %i\n", a);
	
	return 0;
}
