
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

int* getBFS(int root) {
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

void distributedSpanningTree(int rank) {
	int *neigh = getNeighbours(rank);
	int parents[10], parentsRecv[10];
	int recv;
	MPI_Status stat, status;
	MPI_Request sendRequest[10], recvRequest[10], req;

	for (int i = 0; i < 10; i ++)
			parents[i] = -1;

	if (rank != 0) {
    	MPI_Recv(&parents, 10, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &stat);
   		recv = stat.MPI_SOURCE;
   		parents[rank] = recv;
	}
	
	for (int i = 0; i < count; i ++) {
		if (neigh[i] == recv)
			continue;
		MPI_Send(&parents, 10, MPI_INT, neigh[i], 1, MPI_COMM_WORLD);
	}

	for (int i = 0; i < count; i ++) {
		if (neigh[i] == recv)
			continue;
		
        MPI_Recv(&parentsRecv, 10, MPI_INT, neigh[i], 1, MPI_COMM_WORLD, &stat);
		for (int j = 0; j < 10; j ++) {
   				if (parents[j] == -1 && parentsRecv[j] != -1)
   					parents[j] = parentsRecv[j];
   		}
	}

   	if (rank != 0) {
		MPI_Send(&parents, 10, MPI_INT, recv, 1, MPI_COMM_WORLD);
		
        recv = stat.MPI_SOURCE;
   		MPI_Recv(&parents, 10, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &stat);
   		   		recv = stat.MPI_SOURCE;
   	}

   	for (int i = 0; i < count; i ++) {
		if (neigh[i] == recv)
			continue;
		
        MPI_Send(&parents, 10, MPI_INT, neigh[i], 1, MPI_COMM_WORLD);
	}
   	
	if (rank == 9)
	   	for (int i = 0; i < 10; i ++) {
	   		printf("Node: %i, Parent: %i\n", i, parents[i]);
   	}
}


int main(int argc, char * argv[]) {
	int rank, nProcesses;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

	printf("Hello from %i/%i\n", rank, nProcesses);	
	distributedSpanningTree(rank);

	printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	
    return 0;
}