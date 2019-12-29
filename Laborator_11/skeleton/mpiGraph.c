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
	

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Request request;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);
	
	int vector_send[nProcesses];
	int vector_recv[nProcesses];
	for(int i = 0; i < nProcesses; i++){
		vector_send[i] = -1;
		vector_recv[i] = -1;
	}

	//printf("Hello from %i/%i\n", rank, nProcesses);
	for(int i = 0; i < nProcesses; i++){
		if(rank == 0){
			for(int j = 0; j < 3 ; j++){
				MPI_Send(vector_send, nProcesses, MPI_INT, graph[rank + j][1], 0, MPI_COMM_WORLD);
			}
			for(int j = 0; j < 3; j++ ){
				MPI_Recv( vector_recv, nProcesses, MPI_INT, graph[rank + j][1], 0, MPI_COMM_WORLD, &status);
				for(int k = 0; k < nProcesses; k++){
					if(vector_send[k] == -1 && vector_recv[k] != -1){
						vector_send[k] = vector_recv[k];
					}
				}
			}
		}

		if(rank != 0){
			MPI_Recv(vector_recv, nProcesses, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
			int rank_parinte = status.MPI_SOURCE;
			vector_send[rank] = status.MPI_SOURCE;
			
			for(int k = 0; k < nProcesses; k++){
				if(vector_send[k] == -1 && vector_recv[k] != -1){
					vector_send[k] = vector_recv[k];
				}
			}

			for(int j = 0; j < 3 ; j++){
				if(graph[rank * 3 + j][1] != rank_parinte){
					MPI_Send(vector_send, nProcesses, MPI_INT, graph[rank * 3 + j][1], 0, MPI_COMM_WORLD);
				}
			}

			for(int j = 0; j < 3; j++ ){
				if(graph[rank * 3 + j][1] != rank_parinte){	
					MPI_Recv( vector_recv, nProcesses, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
					for(int k = 0; k < nProcesses; k++){
						vector_send[rank] = status.MPI_SOURCE;
						if(vector_send[k] == -1 && vector_recv[k] != -1){
							vector_send[k] = vector_recv[k];
						}
					}
				}
			}

			MPI_Send(vector_send, nProcesses, MPI_INT, rank_parinte, 0, MPI_COMM_WORLD);

		}
	}
	if(rank == 0){
		for(int i = 0; i < nProcesses; i++){
			printf("%d ",vector_send[i]);
		}
		printf("\n");
	}

	printf("Bye from %i/%i\n", rank, nProcesses);
	MPI_Finalize();
	return 0;
}