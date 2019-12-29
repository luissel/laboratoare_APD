#include<mpi.h>
#include<stdio.h>

/**
 * @author cristian.chilipirea
 * Run: mpirun -np 10 ./a.out
 */

int graph[][2] = { { 0, 1 }, { 0, 4 }, { 0, 5 }, 
                  { 1, 0 }, { 1, 2 }, { 1, 6 }, 
                  { 2, 1 }, { 2, 3 }, { 2, 7 },
                  { 3, 2 }, { 3, 4 }, { 3, 8 },
                  { 4, 0 }, { 4, 3 }, { 4, 9 },
                  { 5, 0 }, { 5, 7 }, { 5, 8 },
                  { 6, 1 }, { 6, 8 }, { 6, 9 },
                  { 7, 2 }, { 7, 5 }, { 7, 9 },
                  { 8, 3 }, { 8, 5 }, { 8, 6 },
                  { 9, 4 }, { 9, 6 }, { 9, 7 } };

int main(int argc, char * argv[]) {
	  int rank;
    int nProcesses;
    int* probe = NULL, neighboursCount;
    probe = malloc(10 * sizeof(int));

    MPI_Init(&argc, &argv);
    MPI_Status status;
    MPI_Request request;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
    int leader = rank;

    for (int k = 0; k < nProcesses; k++) {
        for ( int i = 0; i < 30; i++) {
            if (graph[i][0] == rank) {
              MPI_Send(&leader, 1, MPI_INT, graph[i][1], 0, MPI_COMM_WORLD);
            }
        }

        int leaderRecv;
        for ( int i = 0; i < 30; i++) {
            if (graph[i][0] == rank) {
              MPI_Recv(&leaderRecv, 1, MPI_INT, graph[i][1], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            } 
        }

        if (leaderRecv > leader) {
          leader = leaderRecv;
        }
    }

    printf("Sunt %d si am liderul: %d\n ", rank, leader);
    //printf("Bye from %i/%i\n", rank, nProcesses);
    MPI_Finalize();
    return 0;
}