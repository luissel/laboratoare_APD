#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <complex.h>

typedef double complex cplx;
int numThreads, N;
double *originals;
cplx *buf;

void* threadFunction(void *var) {
	int thread_id = *(int*)var;
	// calculez indicii pentru fiecare thread
	int start = (N / numThreads) * thread_id ;
	int end = fmin(ceil((double) N / (double) numThreads) * (thread_id + 1), N);

	for (int k = start; k < end; k++) {
		for (int i = 0; i < N; i++) {
			cplx exp = cexp(-I * 2 * M_PI * k * i / N);
			buf[k] += originals[i] * exp;
		}
	}
	
	return 0;
}

void init(double originals[]) {
	pthread_t tid[numThreads];
	int thread_id[numThreads];

	for(int i = 0; i < numThreads; i++)
		thread_id[i] = i;

	for(int i = 0; i < numThreads; i++)
		pthread_create(&(tid[i]), NULL, threadFunction, &(thread_id[i]));

	for(int i = 0; i < numThreads; i++) 
		pthread_join(tid[i], NULL);
}

 
void show(FILE *output, cplx buf[]) {
	fprintf(output, "%d\n", N);
	for (int i = 0; i < N; i++)
		fprintf(output, "%lf %lf\n", creal(buf[i]), cimag(buf[i]));
}

int main(int argc, char * argv[]) {
	if (argc < 3) {
		fprintf(stdout, "Usage: %s <inputFileName> <outputFileName> <numThreads>\n", argv[0]);
		exit(1);
	}

	FILE *inputValues = fopen(argv[1], "r");
	if (inputValues == NULL) {
		fprintf(stdout, "Failed to open the file\n.");
		exit(1);
	}

	FILE *outputValues = fopen(argv[2], "wt");
	if (outputValues == NULL) {
		fprintf(stdout, "Failed to open the file\n.");
		exit(1);
	}

	numThreads = atoi(argv[3]);

	int ret1 = fscanf(inputValues, "%d", &N);
	if (ret1 == EOF) {
		fprintf(stdout, "Failed to open the file\n.");
		exit(1);
	}

	originals = malloc(N * sizeof(double));
	buf = calloc(N, sizeof(cplx));

	// citesc datele din fisier
	for (int i = 0; i < N; ++i) {
		int ret2 = fscanf(inputValues, "%lf", &originals[i]);
		if (ret2 == EOF) {
			fprintf(stdout, "Failed to open the file\n.");
			exit(1);
		}
	}

	// creez thread-urile
	init(originals);
	// afisez rezultatele obtinute
	show(outputValues, buf);

	free(buf);
	free(originals);

	fclose(inputValues);
	fclose(outputValues);

	return 0;
}
