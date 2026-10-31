#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ITERATIONS 1000000

static long long counter = 0;
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < ITERATIONS; i++) {
#ifdef USE_MUTEX
        pthread_mutex_lock(&mutex);
#endif
        counter++;   /* КРИТИЧЕСКАЯ СЕКЦИЯ */
#ifdef USE_MUTEX
        pthread_mutex_unlock(&mutex);
#endif
    }
    return NULL;
}

int main(void) {
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    printf("counter = %lld (expected %lld)\n",
           counter, (long long)NUM_THREADS * ITERATIONS);
    return 0;
}