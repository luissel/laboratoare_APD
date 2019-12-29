#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

int NReps;
int printLevel;
int N;
int* v;
int *vQSort;
int *vNew;
int P = 4;

static pthread_barrier_t barrier;

void compareVectors(int * a, int * b) {
	// DO NOT MODIFY
	int i;
	for(i = 0; i < N; i++) {
		if(a[i]!=b[i]) {
			printf("Sorted incorrectly\n");
			return;
		}
	}
	printf("Sorted correctly\n");
}

void displayVector(int * v) {
	// DO NOT MODIFY
	int i;
	int displayWidth = 2 + log10(v[N-1]);
	for(i = 0; i < N; i++) {
		printf("%*i", displayWidth, v[i]);
		if(!((i+1) % 20))
			printf("\n");
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}

void getArgs(int argc, char **argv){
	if(argc < 4) {
		printf("Not enough paramters: ./program N NReps printLevel\nprintLevel: 0=no, 1=some, 2=verbouse\n");
		exit(1);
	}
	N = atoi(argv[1]);
	NReps = atoi(argv[2]);
	printLevel = atoi(argv[3]);
}

void init(){
	int i;
	v = malloc(sizeof(int) * N);
	vQSort = malloc(sizeof(int) * N);
	vNew = malloc(sizeof(int) * N);
	if(v == NULL) {
		printf("malloc failed!");
		exit(1);
	}

	// generate the vector v with random values
	// DO NOT MODIFY
	srand(42);
	for(i = 0; i < N; i++)
		v[i] = rand()%N;
}

void printPartial(){
	int i;
	compareVectors(v, vQSort);
}

void printAll(){
	displayVector(v);
	displayVector(vQSort);
	compareVectors(v, vQSort);
}

void print(){
	if(printLevel == 0)
		return;
	else if(printLevel == 1)
		printPartial();
	else
		printAll();
}

void* threadFunction(void *var){
    int sorted = 0, aux, w;
    int thread_id = *(int*)var;
    int start = (N / P) * thread_id;
	int end = fmin(ceil(N / P) * (thread_id + 1), N);
	
    for (int i = 0; i < N; i++) {
		if (start % 2 != 0) {
			start++;
		}

		if (end % 2 != 0) {
			end++;
		}

		for (int j = fmin(0, start); j < fmax(end, N - 1); j += 2) {
			if (v[j] > v[j+ 1]) {
				aux = v[j];
				v[j] = v[j + 1];
				v[j + 1] = aux;
			}
		}

		pthread_barrier_wait(&barrier);
        
		if (start % 2 == 0) {
			start--;
		}

		if (end % 2 == 0) {
			end--;
		}

		for (int j = fmin(1, start); j < fmax(end, N - 1); j += 2) {
			if (v[j] > v[j + 1]) {
				aux = v[j];
				v[j] = v[j +1];
				v[j + 1] = aux;
			}
		}

		pthread_barrier_wait(&barrier);
    }
}

int main(int argc, char *argv[]) {
	int i, j, aux;
	getArgs(argc, argv);
	init();

	// make copy to check it against qsort
	// DO NOT MODIFY
	for(i = 0; i < N; i++)
		vQSort[i] = v[i];
	qsort(vQSort, N, sizeof(int), cmp);

	// sort the vector v
	// PARALLELIZE ME
	pthread_t tid[P];
	int thread_id[P];
    pthread_barrier_init(&barrier, NULL, P);

	for (i = 0; i < P; i++) {
		thread_id[i] = i;
	}

	for(i = 0; i < P; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));
	}

	for(i = 0; i < P; i++) {
		pthread_join(tid[i], NULL);
	}

	pthread_barrier_destroy(&barrier);

	print();

	return 0;
}
