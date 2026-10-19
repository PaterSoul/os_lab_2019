#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    printf("=== РОДИТЕЛЬ: Я запускаюсь! (мой PID = %d) ===\n", getpid());
    
    // Создаём дочерний процесс
    pid_t pid = fork();
    
    if (pid < 0) {
        // Ошибка при создании процесса
        perror("fork не сработал");
        return 1;
    }
    else if (pid == 0) {
        // МЫ ВНУТРИ ДОЧЕРНЕГО ПРОЦЕССА
        printf("=== ДОЧЕРНИЙ: Я создан! (мой PID = %d) ===\n", getpid());
        printf("=== ДОЧЕРНИЙ: Сейчас запущу sequential_min_max ===\n");
        
        // ЗАМЕНЯЕМ себя на программу sequential_min_max
        // Параметры: "42" - seed, "10" - размер массива
        execlp("./sequential_min_max", "sequential_min_max", "42", "10", NULL);
        
        // Если мы дошли сюда — значит execlp НЕ сработал
        perror("execlp не сработал");
        exit(1);
    }
    else {
        // МЫ ВНУТРИ РОДИТЕЛЬСКОГО ПРОЦЕССА
        printf("=== РОДИТЕЛЬ: Жду завершения дочернего (PID = %d) ===\n", pid);
        
        // Ждём, пока дочерний процесс закончит работу
        int status;
        waitpid(pid, &status, 0);
        
        // Проверяем, как завершился дочерний процесс
        if (WIFEXITED(status)) {
            printf("=== РОДИТЕЛЬ: Дочерний завершился со статусом %d ===\n", WEXITSTATUS(status));
        }
        
        printf("=== РОДИТЕЛЬ: Я тоже завершаюсь ===\n");
    }
    
    return 0;
}


