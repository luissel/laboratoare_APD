#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

int m, n;
int **a;
int P = 4;

pthread_barrier_t barrier;



int cmp (const void * b, const void * c) {
	return ( *(int*)b - *(int*)c );
}

int cmpDesc (const void * b, const void * c) {
	return ( *(int*)c - *(int*)b );
}

void sortLine(int* line) {
	qsort(line, n, sizeof(int), cmp);
}

void sortLineDesc(int* line) {
	qsort(line, n, sizeof(int), cmpDesc);
}

void sortCol(int col) {
	int *temp = malloc(sizeof(int) * n);

	for (int i = 0; i < n; ++i) {
		temp[i] = a[i][col];
	}

	qsort(temp, m, sizeof(int), cmp);

	for (int i = 0; i < n; ++i) {
		a[i][col] = temp[i];
	}
}

void sortColDesc(int col) {
	int *temp = malloc(sizeof(int) * n);

	for (int i = 0; i < n; ++i) {
		temp[i] = a[i][col];
	}

	qsort(temp, m, sizeof(int), cmpDesc);
	
	for (int i = 0; i < n; ++i) {
		a[i][col] = temp[i];
	}
}

void* threadFunction(void *args) {
	int thread_id = *(int*)args;
	int start = ceil(m / P) * thread_id;
	int end =  fmin(ceil(m / P) * (thread_id + 1), m);

	for (int j = 1; j < m; j = j * 2) {
    	for (int i = start; i < end; ++i) {
	    	if (i % 2 == 0){
	    		sortLine(a[i]);
	    	}
	    	if (i % 2 != 0){
	    		sortLineDesc(a[i]);
	    	}
    	}

    	pthread_barrier_wait(&barrier);

	    for (int i = 0; i < n; ++i) {
	    	sortCol(i);
	    }
    }
	return 0;
}

int main(int argc, char **argv) {
	pthread_t tid[P];
	int thread_id[P];
	pthread_barrier_init(&barrier, NULL, P);
	
	scanf("%d", &m);
	scanf("%d", &n);

	a = malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++) {
        a[i] = malloc(sizeof(int) * m);
    }

	for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &a[i][j]);
        }
    }

    for(int i = 0; i < P; i++) {
		thread_id[i] = i;
	}

	for (int i = 0; i < P; ++ i) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for(int i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&barrier);

	for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d\t", a[i][j]);
        }
        printf("\n");
    }


	return 0;
}
