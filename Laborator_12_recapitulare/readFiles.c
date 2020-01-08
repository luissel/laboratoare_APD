#include<string.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<pthread.h>

/**
 * @author cristian.chilipirea
 * Create files using
 * dd if=/dev/urandom of=1.txt bs=1024 count=102400
 * dd if=/dev/urandom of=2.txt bs=1024 count=102400
 */
 
int n = 1024*1024*100;
int p = 4;
char buf1[104857600];
char buf2[104857600];
FILE * f;
FILE * g;

void *threadFunction(void *args){
    int thread_id = *(int *)args;

	if (thread_id == 0) {
		fread(buf1,sizeof(char),n,f);
	} else {
		fread(buf2,sizeof(char),n,g);
	}
}

int main(int argc, char * argv[]) {
	f = fopen("1.txt","rb");
	g = fopen("2.txt","rb");
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
	fclose(f);
	fclose(g);
	return 0;
}