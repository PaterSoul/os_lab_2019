#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>  // Добавлено для waitpid

int main(int argc, char *argv[]) {
    // Проверка на наличие достаточного количества аргументов
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <seed> <arraysize>\n", argv[0]);
        return 1;
    }

    // Запуск приложения sequential_min_max
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1; // Ошибка при создании процесса
    } else if (pid == 0) {
        // Дочерний процесс
        execl("./sequential_min_max", "sequential_min_max", argv[1], argv[2], (char *)NULL);
        perror("execl failed"); // Если execl вернется, произошла ошибка
        return 1; // Ошибка при выполнении execl
    } else {
        // Родительский процесс
        int status;
        waitpid(pid, &status, 0); // Ожидание завершения дочернего процесса
        if (WIFEXITED(status)) {
            printf("sequential_min_max exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("sequential_min_max did not terminate normally\n");
        }
    }
    return 0; // Успешное завершение программы
}