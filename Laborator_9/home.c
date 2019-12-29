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

    int a = -1;
    if (rank == 0) {
        a = 1;
        MPI_Send(&a, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&a, 1, MPI_INT, 0, 1,  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    

    printf ("Hello from task %d on %s with number %d!\n", rank, hostname, a);

    if (rank == MASTER)
        printf("MASTER: Number of MPI tasks is: %d\n",numtasks);
    
    MPI_Finalize();

}