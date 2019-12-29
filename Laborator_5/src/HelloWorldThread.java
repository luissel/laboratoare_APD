public class HelloWorldThread implements Runnable {

        static int counter = 0;
        public void run() {

            System.out.println("Hello World!");
            synchronized (this){
                counter = counter + 1;
                synchronized (this) {
                    System.out.println("Thread id: " + counter);
                }
            }
        }

}