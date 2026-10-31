#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#include "find_min_max.h"
#include "utils.h"

volatile pid_t child1_pid = -1;
volatile pid_t child2_pid = -1;

// Обработчик SIGALRM: убивает детей по таймауту
void alarm_handler(int sig) {
    (void)sig;
    printf("\n[TIMEOUT] Time is out! Sending SIGKILL to children...\n");
    if (child1_pid > 0) kill(child1_pid, SIGKILL);
    if (child2_pid > 0) kill(child2_pid, SIGKILL);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s seed arraysize [--timeout N] [by_files]\n", argv[0]);
        return 1;
    }

    int seed = atoi(argv[1]);
    int array_size = atoi(argv[2]);
    int timeout_seconds = 0;
    int use_files = 0;

    // Парсинг именованных аргументов
    for (int i = 3; i < argc; i++) {
        if (strcmp(argv[i], "--timeout") == 0 && i + 1 < argc) {
            timeout_seconds = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "by_files") == 0) {
            use_files = 1;
        }
    }

    int *array = malloc(array_size * sizeof(int));
    GenerateArray(array, array_size, seed);

    printf("Array size: %d, seed: %d", array_size, seed);
    if (timeout_seconds > 0) printf(", timeout: %d", timeout_seconds);
    printf("\n");

    int pipefd[2];
    if (!use_files) pipe(pipefd);

    signal(SIGALRM, alarm_handler);

    // Дочерний процесс 1
    child1_pid = fork();
    if (child1_pid == 0) {
        //sleep(5); // ВРЕМЕННО, для демонстрации! Потом убрать!
        struct MinMax r = GetMinMax(array, 0, array_size / 2 - 1);
        if (use_files) {
            FILE *f = fopen("process1_result.txt", "w");
            fprintf(f, "%d %d", r.min, r.max);
            fclose(f);
        } else {
            write(pipefd[1], &r, sizeof(struct MinMax));
        }
        exit(0);
    }

    // Дочерний процесс 2
    child2_pid = fork();
    if (child2_pid == 0) {
        //sleep(5); // ВРЕМЕННО, для демонстрации! Потом убрать!
        struct MinMax r = GetMinMax(array, array_size / 2, array_size - 1);
        if (use_files) {
            FILE *f = fopen("process2_result.txt", "w");
            fprintf(f, "%d %d", r.min, r.max);
            fclose(f);
        } else {
            write(pipefd[1], &r, sizeof(struct MinMax));
        }
        exit(0);
    }

    // Родитель
    if (!use_files) close(pipefd[1]);

    if (timeout_seconds > 0) alarm(timeout_seconds);

    // Неблокирующее ожидание детей
    int finished_count = 0;
    while (finished_count < 2) {
        int status;
        pid_t p = waitpid(-1, &status, WNOHANG);
        if (p > 0) {
            finished_count++;
            if (WIFSIGNALED(status))
                printf("[INFO] Child %d killed by signal %d\n", p, WTERMSIG(status));
            else
                printf("[INFO] Child %d finished\n", p);
        } else if (p == 0) {
            usleep(50000);
        } else {
            break;
        }
    }

    if (timeout_seconds > 0) alarm(0);

    struct MinMax r1, r2;
    if (use_files) {
        FILE *f1 = fopen("process1_result.txt", "r");
        FILE *f2 = fopen("process2_result.txt", "r");
        fscanf(f1, "%d %d", &r1.min, &r1.max);
        fscanf(f2, "%d %d", &r2.min, &r2.max);
        fclose(f1); fclose(f2);
        remove("process1_result.txt");
        remove("process2_result.txt");
    } else {
        read(pipefd[0], &r1, sizeof(struct MinMax));
        read(pipefd[0], &r2, sizeof(struct MinMax));
        close(pipefd[0]);
    }

    printf("Global Min: %d\n", (r1.min < r2.min) ? r1.min : r2.min);
    printf("Global Max: %d\n", (r1.max > r2.max) ? r1.max : r2.max);

    free(array);
    return 0;
}