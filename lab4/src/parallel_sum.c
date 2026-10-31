#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>

#include "libsum.h"
#include "utils.h"

static double get_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (double)tv.tv_sec + (double)tv.tv_usec / 1e6;
}

int main(int argc, char **argv) {
    int threads_num = 1;
    uint32_t seed = 0;
    uint32_t array_size = 1000;

    /* Парсинг именованных аргументов */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--threads_num") == 0 && i + 1 < argc) {
            threads_num = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc) {
            seed = (uint32_t)atoi(argv[++i]);
        } else if (strcmp(argv[i], "--array_size") == 0 && i + 1 < argc) {
            array_size = (uint32_t)atoi(argv[++i]);
        }
    }

    if (threads_num < 1 || array_size < 1) {
        printf("Usage: %s --threads_num N --seed N --array_size N\n", argv[0]);
        return 1;
    }

    /* Генерация массива — ВНЕ замера времени */
    int *array = malloc((size_t)array_size * sizeof(int));
    if (array == NULL) {
        printf("Error: cannot allocate array\n");
        return 1;
    }
    GenerateArray(array, array_size, seed);

    pthread_t *threads = malloc((size_t)threads_num * sizeof(pthread_t));
    struct SumArgs *args = malloc((size_t)threads_num * sizeof(struct SumArgs));
    if (threads == NULL || args == NULL) {
        printf("Error: cannot allocate thread structures\n");
        free(array);
        return 1;
    }

    uint32_t chunk = array_size / (uint32_t)threads_num;

    double t1 = get_time();   /* старт замера */

    for (int i = 0; i < threads_num; i++) {
        args[i].array = array;
        args[i].start = (uint32_t)i * chunk;
        args[i].end = (i == threads_num - 1) ? array_size : (uint32_t)(i + 1) * chunk;
        args[i].local_sum = 0;
        if (pthread_create(&threads[i], NULL, thread_sum, &args[i]) != 0) {
            printf("Error: pthread_create failed for thread %d\n", i);
            free(array); free(threads); free(args);
            return 1;
        }
    }

    long long total = 0;
    for (int i = 0; i < threads_num; i++) {
        pthread_join(threads[i], NULL);
        total += args[i].local_sum;
    }

    double t2 = get_time();   /* конец замера */

    printf("Threads: %d, Seed: %u, Array size: %u\n", threads_num, seed, array_size);
    printf("Sum: %lld\n", total);
    printf("Time: %.6f seconds\n", t2 - t1);

    free(array);
    free(threads);
    free(args);
    return 0;
}