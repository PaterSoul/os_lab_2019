#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
    long long start;   /* первое число участка */
    long long end;     /* последнее число участка (включительно) */
    long long mod;
    long long local_result;
} ThreadArgs;

static long long global_result = 1;
static pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Поток: перемножает числа своего участка по модулю mod */
void *worker(void *arg) {
    ThreadArgs *a = (ThreadArgs *)arg;
    long long local = 1;
    for (long long i = a->start; i <= a->end; i++) {
        local = (local * (i % a->mod)) % a->mod;
    }
    /* Синхронизация результата через мьютекс */
    pthread_mutex_lock(&result_mutex);
    global_result = (global_result * local) % a->mod;
    pthread_mutex_unlock(&result_mutex);
    return NULL;
}

int main(int argc, char *argv[]) {
    long long k = -1, mod = -1;
    int pnum = -1;

    /* Парсинг: -k 10  --pnum=4  --mod=10 (и вариант через пробел) */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            k = atoll(argv[++i]);
        } else if (strncmp(argv[i], "--pnum=", 7) == 0) {
            pnum = atoi(argv[i] + 7);
        } else if (strcmp(argv[i], "--pnum") == 0 && i + 1 < argc) {
            pnum = atoi(argv[++i]);
        } else if (strncmp(argv[i], "--mod=", 6) == 0) {
            mod = atoll(argv[i] + 6);
        } else if (strcmp(argv[i], "--mod") == 0 && i + 1 < argc) {
            mod = atoll(argv[++i]);
        }
    }

    if (k < 0 || mod < 1 || pnum < 1) {
        printf("Usage: %s -k K --pnum=N --mod=M\n", argv[0]);
        return 1;
    }
    if (k == 0) {
        printf("Result: %lld\n", 1 % mod);   /* 0! = 1 */
        return 0;
    }

    pthread_t *threads = malloc(pnum * sizeof(pthread_t));
    ThreadArgs *args = malloc(pnum * sizeof(ThreadArgs));
    if (threads == NULL || args == NULL) {
        printf("Error: cannot allocate memory\n");
        return 1;
    }

    /* Делим диапазон 1..k на pnum участков */
    long long chunk = k / pnum;
    for (int i = 0; i < pnum; i++) {
        args[i].start = i * chunk + 1;
        args[i].end = (i == pnum - 1) ? k : (i + 1) * chunk;
        args[i].mod = mod;
        args[i].local_result = 1;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    for (int i = 0; i < pnum; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Result: %lld\n", global_result);

    free(threads);
    free(args);
    pthread_mutex_destroy(&result_mutex);
    return 0;
}