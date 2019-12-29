#include<mpi.h>
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
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

int count = 0;
int* getNeighbours(int node) {
 	int *neighbours, no = 0;
 	count = 0;

 	for (int i = 0; i < 30; i ++) {
 		if (graph[i][0] == node)
 			no++;
 	}

 	neighbours = (int *)malloc(no * sizeof(int));

 	for (int i = 0; i < 30; i ++) {
 		if (graph[i][0] == node)
 			neighbours[count++] = graph[i][1];
 	}

 	return neighbours;
}

int* firstStepBFS(int node) {
    int* visited = malloc(10 * sizeof(*visited));
    int queue[10];
    int start = 0;
    int end = -1;
    
    int i, crtNode;
    int* neigh = NULL;

    for (int i = 0; i < 10; i ++) {
		visited[i] = -1;
	}

    visited[node] = node;
    neigh = getNeighbours(node);

    for (i = 0; i != count; ++i) {
        visited[neigh[i]] = neigh[i];
		queue[++end] = neigh[i];
    }

    while (end >= start) {
        crtNode = queue[start++];
        neigh  = getNeighbours(crtNode);

        for (i = 0; i != count; ++i) {
            if (visited[neigh[i]] == -1) {
                queue[++end] = neigh[i];
                visited[neigh[i]] = visited[crtNode];
            }
        }
    }

    return visited;
}

void send0To9(int rank)
{
    int i, num, count, sendNext = 1;
    int* nextNodes  = firstStepBFS(rank);
    int* neighbours = getNeighbours(rank);

    if (rank != 0) {
        MPI_Recv(&num, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        if (num == -1)
            sendNext = 0;

        if (rank == 9) {
            printf("Receiver got: %d\n", num);
            sendNext = 0;
        }
    } else {
        num = rand() % 1000;
        printf("Sender sent: %d\n", num);
    }

    if (sendNext == 1) {
        MPI_Send(&num, 1, MPI_INT, nextNodes[9], MPI_TAG_UB, MPI_COMM_WORLD);
        
    }
}

int main(int argc, char* argv[])
{
    int rank, nProcesses;
    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    send0To9(rank);
    printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();

    return 0;
}