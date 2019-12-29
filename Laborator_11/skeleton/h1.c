#include<mpi.h>
#include<stdio.h>
int main(int argc, char * argv[])
{
    int rank;
    int nProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    MPI_Request sendRequest, recvRequest;
    MPI_Status status;
    int a = 99999;
    int b = 0;

    if (rank == 0) {
    	MPI_Isend(&a, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &sendRequest);
    } else {
    	MPI_Irecv(&b, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &recvRequest);
   		MPI_Wait(&recvRequest, &status);
    	printf("Proc %d a primit %d", rank, b);
    }

    MPI_Finalize();
    return 0;
}