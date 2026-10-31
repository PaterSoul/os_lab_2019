#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg) {
    (void)arg;
    printf("Thread 1: locking mutex A...\n");
    pthread_mutex_lock(&mutexA);
    printf("Thread 1: locked A\n");
    sleep(1);   /* даём потоку 2 время занять B */
    printf("Thread 1: trying to lock mutex B...\n");
    pthread_mutex_lock(&mutexB);        /* ЗДЕСЬ НАМЕРТВО ЗАСТРЕВАЕМ */
    printf("Thread 1: locked B (you will never see this)\n");
    pthread_mutex_unlock(&mutexB);
    pthread_mutex_unlock(&mutexA);
    return NULL;
}

void *thread2(void *arg) {
    (void)arg;
    printf("Thread 2: locking mutex B...\n");
    pthread_mutex_lock(&mutexB);
    printf("Thread 2: locked B\n");
    sleep(1);   /* даём потоку 1 время занять A */
    printf("Thread 2: trying to lock mutex A...\n");
    pthread_mutex_lock(&mutexA);        /* ЗДЕСЬ НАМЕРТВО ЗАСТРЕВАЕМ */
    printf("Thread 2: locked A (you will never see this)\n");
    pthread_mutex_unlock(&mutexA);
    pthread_mutex_unlock(&mutexB);
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    printf("If the program hangs below - this is DEADLOCK. Press Ctrl+C to exit.\n");

    pthread_join(t1, NULL);   /* никогда не вернётся */
    pthread_join(t2, NULL);
    return 0;
}