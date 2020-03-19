import java.util.concurrent.*;
import java.util.concurrent.locks.*;

/*
 *   Description:
 *               Demonstration of multithreading, race condition, and critical section controls using ExecutorService for
 *               thread management, and multiple synchronization methods.
 *
 *   Compile:    javac Multithreading.java
 *   Run:        java Multithreading
 *
 *   Output:
 *               "Non-synchronized sum: <A value less than or equal to 1000>
 *               Synchronized sum using locks: 1000
 *               Synchronized sum using "synchronized" keyword: 1000
 *               Synchronized sum using a semaphore with 1 permit: 1000"
 *
 *   Author:     Haya
 *
 */
public class Multithreading {
    static final int THREAD_COUNT = Runtime.getRuntime().availableProcessors();
    static int sum = 0;
    public static void main(String[] args) {
        withoutSync();
        sum = 0;
        withSyncLock();
        sum = 0;
        withSyncKeyword();
        sum = 0;
        withSyncSemaphore();
    }

    static void withoutSync() {
        ExecutorService executor = Executors.newFixedThreadPool(THREAD_COUNT);
        for (int i = 0; i < 1000; i++){
            executor.execute(() -> {
                sum++;
            });
        }
        executor.shutdown();
        while (!executor.isTerminated());
        System.out.println("Non-synchronized sum: " + sum);
    }

    static void withSyncLock() {
        ExecutorService executor = Executors.newFixedThreadPool(THREAD_COUNT);
        Lock lock = new ReentrantLock();
        for (int i = 0; i < 1000; i++){
            executor.execute(() -> {
                lock.lock();
                sum++;
                lock.unlock();
            });
        }
        executor.shutdown();
        while (!executor.isTerminated());
        System.out.println("Synchronized sum using locks: " + sum);
    }

    static void withSyncKeyword() {
        ExecutorService executor = Executors.newFixedThreadPool(THREAD_COUNT);
        Increment incr = new Increment();
        for (int i = 0; i < 1000; i++){
            executor.execute(() -> {
                incr.increment();
            });
        }
        executor.shutdown();
        while (!executor.isTerminated());
        System.out.println("Synchronized sum using \"synchronized\" keyword: " + sum);
    }

    static void withSyncSemaphore() {
        Semaphore semaphore = new Semaphore(1);
        ExecutorService executor = Executors.newFixedThreadPool(THREAD_COUNT);
        for (int i = 0; i < 1000; i++){
            executor.execute(() -> {
                try {
                    semaphore.acquire();
                    sum++;
                } catch (InterruptedException ex) {

                } finally {
                    semaphore.release();
                }
            });
        }
        executor.shutdown();
        while (!executor.isTerminated());
        System.out.println("Synchronized sum using a semaphore with 1 permit: " + sum);
    }

    static class Increment {
        synchronized void increment() {
            sum++;
        }
    }
}