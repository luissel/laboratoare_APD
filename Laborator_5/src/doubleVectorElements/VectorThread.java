package doubleVectorElements;

public class VectorThread implements Runnable{
    int array[];
    int id;
    int P;

    VectorThread(int array[], int id, int P) {
        this.array = array;
        this.id = id;
        this.P = P;
    }

    @Override

    public void run () {
        int N = array.length;
        int frac = N / P;
        int start = id * frac;
        int end = (id + 1) * frac;
        System.out.println("start = " + start + " end = " + end + " thread id = " + id);
        if (id == P - 1) {
            end = N;
        }
        for (int i = start; i < end; ++i) {
            array[i] = array[i] * 2;
        }
    }
}
