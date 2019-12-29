#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER		0

int main (int argc, char *argv[])
{
    int   numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int a = 0;
    MPI_Status stat;

    if (rank != numtasks - 1) {
        a += 2;
        MPI_Send(&a, 1, MPI_INT, numtasks - 1, 0, MPI_COMM_WORLD);
    }

    if (rank == numtasks - 1) {
        for (int i = 0; i < 3; i++) {
            MPI_Recv(&a, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG,  MPI_COMM_WORLD, &stat);
            printf ("Hello from task %d on %s with number %d!\n", stat.MPI_SOURCE, hostname, a);
        }
    }

    MPI_Finalize();
}