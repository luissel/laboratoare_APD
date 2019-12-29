#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER		0

int main (int argc, char *argv[])
{
    int   numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int v[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int a = 2;

    if (rank == 0) {
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&a, 1, MPI_INT, numtasks - 1, 0,  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf ("Hello from task %d on %s with number %d!\n", rank, hostname, a);
    } else if ( rank < numtasks - 1) {
        MPI_Recv(&a, 1, MPI_INT, rank - 1, 0,  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf ("Hello from task %d on %s with number %d!\n", rank, hostname, a);
        
        a += 2;
        MPI_Send(&a, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    if (rank == numtasks - 1) {
        MPI_Recv(&a, 1, MPI_INT, rank - 1, 0,  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf ("Hello from task %d on %s with number %d!\n", rank, hostname, a);
        a += 2;
        MPI_Send(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();
}