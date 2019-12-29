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

    int n = -1;
    if (rank == 0) {
        n = 100;

        for (int i = 1; i < numtasks; ++i) {
            MPI_Send(&n, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
        }

    } else {
        MPI_Recv(&n, 1, MPI_INT, 0, 1,  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    printf ("Hello from task %d on %s with number %d!\n", rank, hostname, n);

    MPI_Finalize();

}