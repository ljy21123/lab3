#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int child_status;

    pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    } else if (pid == 0) {
        // 자식 프로세스
        printf("This is the child process\n");
        exit(0);
    } else {
        // 부모 프로세스
        wait(&child_status);
        printf("Child process exited with status %d\n", WEXITSTATUS(child_status));
    }

    return 0;
}
