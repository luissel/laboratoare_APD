#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank;
    int nProcesses;
    int send = 0, recv = 0;
    int v[8] = {0, 1, 2, 3, 4, 5, 6, 7};

    MPI_Status s;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    send = v[rank];
    for (int i = 1; i < nProcesses; i *= 2) {

        if (rank % (2 * i) == i) {
            MPI_Send(&send, 1, MPI_INT, rank - i, 0, MPI_COMM_WORLD);
        }

        if (rank % (2 * i) == 0) {
            MPI_Recv(&recv, 1, MPI_INT, rank + i, 0, MPI_COMM_WORLD, &s);
            send += recv;
        }
    }

    if (rank == 0) {
        printf("Suma: %d from rank: %d\n", send, rank);
    }

    MPI_Finalize();
    return 0;
}