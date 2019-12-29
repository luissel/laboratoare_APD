#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <complex.h>

typedef double complex cplx;
int numThreads, N;
double *originals;
cplx *buf, *output;

pthread_barrier_t mybarrier;
pthread_barrier_t mybarrier2;

unsigned int bitReverse(unsigned int x) { 
    int aux = 0;
    for (int i = 0; i < log2(N); i++) { 
        aux<<= 1; 
        aux |= (x & 1); 
        x >>= 1; 
    } 

    return aux; 
} 

void fft (int start, int stop, int s_start, int s_end) {   
    for (int stage = s_start; stage <= s_end; ++stage) { 
		// calculez numarul de elemente cu care lucrez in fiecare etapa
		// si aplic formulele pe elemntele respective
        int count = 1 << stage;
		cplx exp = 1;

        for (int j = start; j < count / 2 + start; ++j) { 
            for (int k = j; k < stop; k += count) { 
                cplx t = exp * output[k + count / 2];  
                cplx u = output[k]; 
  
                output[k] += t;
                output[k + count / 2] = u - t;  
            } 
            exp *= cexp(- I * M_PI / (count / 2));; 
        } 
    } 
} 

void* threadFunction1(void *var) {
	int thread_id = *(int*)var;

	if (numThreads == 1) {
		// aplic fft iterativ
		fft(0, N, 1, log2(N));
	} else {
		// calculez indicii pentru fiecare thread
		int start = (N / numThreads) * thread_id ;
		int end = fmin(ceil(N / numThreads) * (thread_id + 1), N);

		fft(start, end, 1, log2(N) - 2);

		pthread_barrier_wait(&mybarrier);

		// pentru penultima etapa, am impartit elementele in 2 jumatati: primul
		// thread face operatiile pe prima jumatate, iar al doilea thread pe
		// cealalta
		fft(0, N / 2, log2(N) - 1, log2(N) - 1);
		
		pthread_barrier_wait(&mybarrier2);

		// calculele finale si scrierea rezultatelor in vectorul de out
		fft(0, N, log2(N), log2(N));	
	}
	return 0;
}

void* threadFunction2(void *var) {
	int thread_id = *(int*)var;
	// calculez indicii pentru fiecare thread
	int start = (N / numThreads) * thread_id ;
	int end = fmin(ceil(N / numThreads) * (thread_id + 1), N);

	fft(start, end, 1, log2(N) - 2);

	pthread_barrier_wait(&mybarrier);

	fft(N / 2, N, log2(N) - 1, log2(N) - 1);
	
	pthread_barrier_wait(&mybarrier2);
	return 0;
}

void* threadFunction3(void *var) {
	int thread_id = *(int*)var;
	// calculez indicii pentru fiecare thread
	int start = (N / numThreads) * thread_id ;
	int end = fmin(ceil(N / numThreads) * (thread_id + 1), N);

	fft(start, end, 1, log2(N) - 2);

	pthread_barrier_wait(&mybarrier);
	return 0;
}

void* threadFunction4(void *var) {
	int thread_id = *(int*)var;
	// calculez indicii pentru fiecare thread
	int start = (N / numThreads) * thread_id ;
	int end = fmin(ceil(N / numThreads) * (thread_id + 1), N);

	fft(start, end, 1, log2(N) - 2);

	pthread_barrier_wait(&mybarrier);
	return 0;
}

void init(double originals[]) {
	pthread_t tid[numThreads];
	int thread_id[numThreads];

	pthread_barrier_init(&mybarrier, NULL, numThreads);
	pthread_barrier_init(&mybarrier2, NULL, 2);

	for(int i = 0; i < numThreads; i++)
		thread_id[i] = i;

	for(int i = 0; i < numThreads; i++) {
		if (i % numThreads == 0) {
			pthread_create(&(tid[i]), NULL, threadFunction1, &(thread_id[i]));
		} else if (i % numThreads == 1) {
			pthread_create(&(tid[i]), NULL, threadFunction2, &(thread_id[i]));
		} else if (i % numThreads == 2) {
			pthread_create(&(tid[i]), NULL, threadFunction3, &(thread_id[i]));
		} else {
			pthread_create(&(tid[i]), NULL, threadFunction4, &(thread_id[i]));
		} 
	}

	for(int i = 0; i < numThreads; i++) 
		pthread_join(tid[i], NULL);

	pthread_barrier_destroy(&mybarrier);
	pthread_barrier_destroy(&mybarrier2);
}

 
void show(FILE *output, cplx buf[]) {
	fprintf(output, "%d\n", N);
	for (int i = 0; i < N; i++)
		fprintf(output, "%lf %lf\n", creal(buf[i]), cimag(buf[i]));
}

int main(int argc, char * argv[]) {
	if (argc < 3) {
		fprintf(stdout,
				"Usage: %s <inputFileName> <outputFileName> <numThreads>\n",
				argv[0]);
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

	buf = calloc(N, sizeof(cplx));
	originals = malloc(N * sizeof(double));
	output = malloc(N * sizeof(cplx));
	
	// citesc datele din fisier
	for (int i = 0; i < N; ++i) {
		int ret2 = fscanf(inputValues, "%lf", &originals[i]);
		if (ret2 == EOF) {
			fprintf(stdout, "Failed to open the file\n.");
			exit(1);
		}
	}

	// bit reversal pe input
    for (unsigned int i = 0; i < N; ++i) { 
        int rev = bitReverse(i); 
        output[i] = originals[rev]; 
    } 

	// creez thread-urile
	init(originals);
	// scriu rezultatul in fisier
	show(outputValues, output);

	free(buf);
	free(originals);
	free(output);

	fclose(inputValues);
	fclose(outputValues);
	return 0;
}

