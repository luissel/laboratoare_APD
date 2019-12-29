public class HelloWord {

    public static void main(String [] args){

        int N = 4, i;
        Thread threads[] = new Thread[N];

        for (i = 0; i < N; ++i) {
            HelloWorldThread aux = new HelloWorldThread();
            threads[i] = new Thread(aux);
        }

        for (i = 0; i < N; ++i) {
            threads[i].start();
        }

        for (i = 0; i < N; ++i) {
            try{
                threads[i].join();
            } catch(InterruptedException e) {
                e.printStackTrace();
            }
        }


    }
}
