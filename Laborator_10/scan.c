#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank;
    int nProcesses;
    int send = 0, recv = 0;

    MPI_Status s;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    send = rank;
    for (int i = 1; i < nProcesses; i *= 2) {
        
        if (rank + i < nProcesses) {
            MPI_Send(&send, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD);
        }

        if (rank - i >= 0) {
            MPI_Recv(&recv, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD, &s);
            send += recv;
        }
    }

    if (rank == nProcesses - 1) {
        printf("Suma: %d from rank: %d\n", send, rank);
    }

    MPI_Finalize();
    return 0;
}