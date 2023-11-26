#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigintHandler(int sig_num) {
    // 시그널 번호를 출력하고 프로그램을 종료하지 않음
    printf("\nCannot be terminated using Ctrl+C\n");
    fflush(stdout);
}

int main() {
    // SIGINT 시그널에 대한 핸들러 설정
    signal(SIGINT, sigintHandler);

    // 무한 루프로 프로그램이 계속 실행되게 함
    while (1) {
        printf("Sleeping for 1 second...\n");
        sleep(1);
    }

    return 0;
}
