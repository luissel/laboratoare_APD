#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

/**
 * @author cristian.chilipirea
 *
 */

int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	printf("Hello from %i/%i\n", rank, nProcesses);

 // This code is run by a single process
		int polynomialSize;
		int n;
		int i;
		int j;
		float x;
		MPI_Status s;
		FILE * polFunctionFile = fopen("a1.txt","rt");
		fscanf(polFunctionFile, "%i", &polynomialSize);
		float * a = malloc(sizeof(float)*polynomialSize);
		
		for(i=0;i<polynomialSize;i++) {
			fscanf(polFunctionFile, "%f", a+i);
		}
		
		fclose(polFunctionFile);

		for(x = 0; x < 3; x += 1) {
			float sum = 0;
			float xPowI = 1;

			if (rank == 0) {
				sum += xPowI * a[rank];
				xPowI *= x;
				
				MPI_Send(&sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
				MPI_Send(&xPowI, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
			} else if (rank == nProcesses - 1) {
				MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &s);
				MPI_Recv(&xPowI, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &s);
				
				sum += xPowI * a[rank];
				xPowI *= x; 
			} else {
				MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &s);
				MPI_Recv(&xPowI, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &s);
				
				sum += xPowI * a[rank];
				xPowI *= x; 
				
				MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
				MPI_Send(&xPowI, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
			}

			if (x == 2 && rank == nProcesses - 1) {
				printf("sum: %f from rank: %d\n", sum, rank);
			}
		}

	
	MPI_Finalize();
	return 0;
}