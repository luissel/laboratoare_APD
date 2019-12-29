#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER		0

int main (int argc, char *argv[])
{
    int numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int v[100];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    if (rank == 0) {
        for (int i = 0; i < 100; ++i) {
            v[i] = i;
        }
    }
 
    MPI_Scatter(v, 25, MPI_INT, v, 25, MPI_INT, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < 25; i++) {
        v[i] += 42;
    }

    MPI_Gather(v, 25, MPI_INT, v, 25, MPI_INT, 0, MPI_COMM_WORLD);  

    if (rank == 0) {
        printf ("Hello from task %d on %s\n", rank, hostname);
       
        for (int i = 0; i < 100; ++i) {
            printf("%d ", v[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
}