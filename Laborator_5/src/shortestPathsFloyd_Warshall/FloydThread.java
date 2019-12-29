package shortestPathsFloyd_Warshall;

public class FloydThread implements  Runnable{
    int graph[][];
    int id;
    int P;

    FloydThread(int graph[][], int id, int P) {
        this.graph = graph;
        this.id = id;
        this.P = P;
    }

    @Override
    public void run() {
        int N = graph.length;
        System.out.println("N = " + N);
        int frac = N / P;
        int start = id * frac;
        int end = (id + 1) * frac;
        if (id == P - 1) {
            end = N;
        }
        for (int k = 0; k < 5; k++) {
            for (int i = start; i < end; i++) {
                for (int j = 0; j < 5; j++) {
                    graph[i][j] = Math.min(graph[i][k] + graph[k][j], graph[i][j]);
                }
            }
        }
    }
}
