#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define N 10000

/**
 * @author cristian.chilipirea
 *
 */


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
	}
	printf("\n");
}

int cmp(const void *a, const void *b) {
	// DO NOT MODIFY
	int A = *(int*)a;
	int B = *(int*)b;
	return A-B;
}
 
int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);
	MPI_Status s;
	int temp;
	int recv = 0;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
//	printf("Hello from %i/%i\n", rank, nProcesses);

	if(rank == 0) { // This code is run by a single process
		int sorted = 0;
		int aux;
		int *v = (int*)malloc(sizeof(int)*N);
		int *vQSort = (int*)malloc(sizeof(int)*N);
		int i;

		// generate the vector v with random values
		// DO NOT MODIFY
		srand(42);
		for(i = 0; i < N; i++)
			v[i] = rand()%N;

		// make copy to check it against qsort
		// DO NOT MODIFY
		for(i = 0; i < N; i++)
			vQSort[i] = v[i];
		qsort(vQSort, N, sizeof(int), cmp);

		/* // sort the vector v
		// REPLACE THIS WITH YOUR SORT
		while(!sorted) {
			sorted = 1;
			for(i = 0; i < N-1; i++) {
				if(v[i] > v[i + 1]) {
					aux = v[i];
					v[i] = v[i + 1];
					v[i + 1] = aux;
					sorted = 0;
				}
			}
		 } */

		//displayVector(vQSort);

		temp = v[0];
		for (int i = 1; i < nProcesses - rank; ++i) {
			if (temp > v[i]) {
				MPI_Send(&temp, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
				temp = v[i];
			} else {
				MPI_Send(&v[i], 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			}
		}

		printf("Number: %d from rank: %d\n", temp, rank);
	} else if (rank == nProcesses - 1){
		
		MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &s);
		printf("Number: %d from rank: %d\n", recv, rank);
	} else {
		MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &s);
		temp = recv;
		
		for (int i = 1; i < nProcesses - rank; ++i) {
			MPI_Recv(&recv, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &s);
			
			if (temp > recv) {
				MPI_Send(&temp, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
				temp = recv;
			} else {
				MPI_Send(&recv, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			}
		}
		
		printf("Number: %d from rank: %d\n", temp, rank);
	}

	// 	compareVectors(v, vQSort);
	// }

	MPI_Finalize();
	return 0;
}