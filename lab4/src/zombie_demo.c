#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Parent PID: %d\n", getpid());

    for (int i = 0; i < 3; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Child %d (PID: %d) exiting immediately\n", i + 1, getpid());
            exit(0);
        }
    }

    printf("\nParent sleeps 15 sec. Children are now ZOMBIES.\n");
    printf("In another terminal run: ps -eo pid,ppid,stat,cmd | grep Z\n\n");
    sleep(15);

    printf("Parent wakes up. Reaping zombies via wait()...\n");
    for (int i = 0; i < 3; i++) {
        int status;
        pid_t p = wait(&status);
        printf("Reaped zombie PID: %d\n", p);
    }

    printf("All zombies cleaned up.\n");
    return 0;
}