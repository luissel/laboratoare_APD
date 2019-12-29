#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int N, P;
int **mat;
int column;

pthread_barrier_t barrier;

void getArgs(int argc, char **argv)
{
	if(argc < 3) {
		printf("Not enough paramters: ./program N NReps printLevel\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	N = atoi(argv[1]);
	P = atoi(argv[2]);
}

int compareColumn (const void* line1, const void* line2) {
	const int* l1 = *(const int**)line1;
	const int* l2 = *(const int**)line2;

	return l2[column] - l1[column]; 
}

int sortAsc(const void* a, const void* b) {
	int x = *(int *)a;
 	int y = *(int *)b;

  	if (x<y) return -1; 
  	if (x>y) return 1; 
  	return 0;
}

int sortDesc(const void* a, const void* b) {
	int x = *(int *)a;
  	int y = *(int *)b;

  	if (x<y) return 1; 
  	if (x>y) return -1; 
  	return 0;
}

double Log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2 );  
}

int** getTranspose(int** mt) {
	int** res = (int**) malloc (N * sizeof(int*));

	for (int i = 0; i < N; ++i) {
		res[i] = (int*) malloc (N * sizeof(int));
	}

	for (int i = 0; i < N; ++i) {
		for(int j = 0; j < N; ++j) {
			res[i][j] = mt[j][i];
		}
	}

	return res;
}

void* threadFunction(void *var)
{
	int t_id = *(int*)var;

	int start = t_id * ceil((double)N / P);
	int end = fmin((t_id + 1) * ceil((double)N / P), N);

	for (int i = 0; i < ceil(Log2(N)) + 1; ++i) {
		int** transp = getTranspose(mat);
		for (int j = 0; j < N; ++j) {
			qsort(transp[j], N, sizeof(int), sortAsc);
		}

		mat = getTranspose(transp);

		pthread_barrier_wait(&barrier);

		for (int k = start; k < end; ++k) {
			if (k % 2 == 0) {
				qsort(mat[k], N, sizeof(int), sortAsc);
			} else {
				qsort(mat[k], N, sizeof(int), sortDesc);
			}
		}

		pthread_barrier_wait(&barrier);
	}
}

void print(int** mt) {
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			printf("%d ", mt[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char **argv)
{
	int i;

	getArgs(argc, argv);

	mat = (int **) malloc (N * sizeof(int*));

	for (int i = 0; i < N; ++i) {
		mat[i] = (int*) malloc (N * sizeof(int));	
	}

	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			scanf("%d", &mat[i][j]);
		}
	}

	pthread_barrier_init(&barrier, NULL, P);

	pthread_t tid[P];
	int thread_id[P];
	for(i = 0;i < P; i++)
		thread_id[i] = i;

	for(i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	printf("\nMatricea sortata: \n");
	print(mat);
	pthread_barrier_destroy(&barrier);
	return 0;
}
