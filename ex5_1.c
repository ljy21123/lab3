#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        // fork 실패
        fprintf(stderr, "Fork Failed");
        return 1;
    } else if (pid == 0) {
        // 자식 프로세스
        execlp("/bin/ls", "ls", NULL);
    } else {
        // 부모 프로세스
        wait(NULL);
        printf("Child Complete\n");
    }

    return 0;
}
