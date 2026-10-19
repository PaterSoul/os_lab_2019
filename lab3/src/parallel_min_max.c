#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "find_min_max.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s seed arraysize [by_files]\n", argv[0]);
        return 1;
    }

    int seed = atoi(argv[1]);
    int array_size = atoi(argv[2]);
    int use_files = (argc > 3 && strcmp(argv[3], "by_files") == 0);

    // Генерируем массив
    int *array = malloc(array_size * sizeof(int));
    GenerateArray(array, array_size, seed);

    printf("Array size: %d, seed: %d\n", array_size, seed);

    int pipefd[2];
    if (!use_files) {
        pipe(pipefd);
    }

    // Первый дочерний процесс - первая половина массива
    pid_t pid1 = fork();
    if (pid1 == 0) {
        struct MinMax result = GetMinMax(array, 0, array_size / 2 - 1);

        if (use_files) {
            FILE *f = fopen("process1_result.txt", "w");
            fprintf(f, "%d %d", result.min, result.max);
            fclose(f);
        } else {
            write(pipefd[1], &result.min, sizeof(int));
            write(pipefd[1], &result.max, sizeof(int));
        }
        exit(0);
    }

    // Второй дочерний процесс - вторая половина массива
    pid_t pid2 = fork();
    if (pid2 == 0) {
        struct MinMax result = GetMinMax(array, array_size / 2, array_size - 1);

        if (use_files) {
            FILE *f = fopen("process2_result.txt", "w");
            fprintf(f, "%d %d", result.min, result.max);
            fclose(f);
        } else {
            write(pipefd[1], &result.min, sizeof(int));
            write(pipefd[1], &result.max, sizeof(int));
        }
        exit(0);
    }

    // Родительский процесс
    if (!use_files) {
        close(pipefd[1]);
    }

    wait(NULL);
    wait(NULL);

    int global_min, global_max;

    if (use_files) {
        FILE *f1 = fopen("process1_result.txt", "r");
        FILE *f2 = fopen("process2_result.txt", "r");

        int min1, max1, min2, max2;
        fscanf(f1, "%d %d", &min1, &max1);
        fscanf(f2, "%d %d", &min2, &max2);

        fclose(f1);
        fclose(f2);

        global_min = (min1 < min2) ? min1 : min2;
        global_max = (max1 > max2) ? max1 : max2;

        remove("process1_result.txt");
        remove("process2_result.txt");
    } else {
        int min1, max1, min2, max2;
        read(pipefd[0], &min1, sizeof(int));
        read(pipefd[0], &max1, sizeof(int));
        read(pipefd[0], &min2, sizeof(int));
        read(pipefd[0], &max2, sizeof(int));

        global_min = (min1 < min2) ? min1 : min2;
        global_max = (max1 > max2) ? max1 : max2;

        close(pipefd[0]);
    }

    printf("Global Min: %d\n", global_min);
    printf("Global Max: %d\n", global_max);

    free(array);
    return 0;
}