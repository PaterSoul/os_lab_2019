#ifndef LIBSUM_H
#define LIBSUM_H

#include <stdint.h>

struct SumArgs {
    const int *array;
    uint32_t start;
    uint32_t end;
    long long local_sum;
};

/* Считает сумму элементов array[start..end) */
long long Sum(const struct SumArgs *args);

/* Точка входа потока: кладёт результат в local_sum */
void *thread_sum(void *arg);

#endif