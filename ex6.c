#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int mySystem(const char *command) {
    pid_t pid = fork();

    if (pid < 0) {
        // fork 실패
        return -1;
    } else if (pid == 0) {
        // 자식 프로세스: 전달받은 명령 실행
        execlp("/bin/sh", "sh", "-c", command, NULL);
        exit(EXIT_FAILURE); // exec가 실패할 경우
    } else {
        // 부모 프로세스: 자식 프로세스 종료 대기
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    char command[1024] = {0};
    for (int i = 1; i < argc; i++) {
        strcat(command, argv[i]);
        strcat(command, " ");
    }

    printf("Executing command: %s\n", command);
    mySystem(command);

    return 0;
}
