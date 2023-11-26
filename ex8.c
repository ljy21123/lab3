#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int pipefds[2];
    char buf[30];
    char msg[] = "Hello World!";

    // 파이프 생성
    if (pipe(pipefds) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // 자식 프로세스
        close(pipefds[1]); // 쓰기용 파이프 닫기
        read(pipefds[0], buf, sizeof(buf)); // 부모로부터 데이터 읽기
        printf("Child received: %s\n", buf);
        close(pipefds[0]); // 읽기용 파이프 닫기
    } else {
        // 부모 프로세스
        close(pipefds[0]); // 읽기용 파이프 닫기
        write(pipefds[1], msg, strlen(msg) + 1); // 자식에게 데이터 쓰기
        close(pipefds[1]); // 쓰기용 파이프 닫기
        wait(NULL); // 자식 프로세스가 종료될 때까지 대기
    }

    return 0;
}
