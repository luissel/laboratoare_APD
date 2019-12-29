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

int* BFS(int root) {
	int *visited = (int *)malloc(10 * sizeof(int));
	int *queue = (int *)malloc(10 * 10 * sizeof(int));
	int start = 0, finish;

	for (int i = 0; i < 10; i ++) {
		visited[i] = -1;
	}

	int *neigh = getNeighbours(root);

	for (int i = 0; i < count; i ++) {
		visited[neigh[i]] = neigh[i];
		queue[i] = neigh[i];
	}

	visited[root] = root;
	finish = count;

	while (start <= finish) {
		int *neigh = getNeighbours(queue[start ++]);

		for (int i = 0; i < count; i ++) {
			if (visited[neigh[i]] == -1) {
				visited[neigh[i]] = visited[queue[start - 1]];
				queue[finish ++] = neigh[i];
			}
		}
	}

	return visited;
}

int main(int argc, char * argv[]) {
	int rank;
	int nProcesses;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	int *neighbours = getNeighbours(rank);

	neighbours = BFS(rank);

	for (int i = 0; i < 10; i ++)
		printf("Fastest route from %i to %i starts in %i\n", rank, i, neighbours[i]);
	
	printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}