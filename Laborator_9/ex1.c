#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER		0

int main (int argc, char *argv[])
{
    int numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int a[100];
    for (int i = 0; i < 100; ++i) {
        a[i] = 0;
    } 

    if (rank == 0) {
        for (int i = 0; i < 100; ++i) {
            a[i] = i;
        }
        MPI_Send(&a, 100, MPI_INT, 1, 1, MPI_COMM_WORLD);
    } else {
        MPI_Recv(&a, 100, MPI_INT, 0, 1,  MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    printf ("Hello from task %d on %s\n", rank, hostname);
    
    for (int i = 0; i < 100; ++i) {
        printf("%d ", a[i]);
    }
    
    
    MPI_Finalize();

}