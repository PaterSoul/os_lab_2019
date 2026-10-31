#include "libsum.h"
#include <stddef.h>

long long Sum(const struct SumArgs *args) {
    if (args == NULL || args->array == NULL || args->start > args->end) {
        return 0;
    }
    long long sum = 0;
    for (uint32_t i = args->start; i < args->end; i++) {
        sum += args->array[i];
    }
    return sum;
}

void *thread_sum(void *arg) {
    struct SumArgs *data = (struct SumArgs *)arg;
    data->local_sum = Sum(data);
    return NULL;
}