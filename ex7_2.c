#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigintHandler(int sig_num) {
    printf("\nCaught signal %d, coming out...\n", sig_num);
    exit(0);
}

int main() {
    struct sigaction sigAct;

    // sigintHandler를 시그널 핸들러로 설정
    sigAct.sa_handler = sigintHandler;
    sigemptyset(&sigAct.sa_mask);
    sigAct.sa_flags = 0;

    // SIGINT 시그널에 대한 핸들러 설정
    sigaction(SIGINT, &sigAct, NULL);

    // 무한 루프로 프로그램이 계속 실행되게 함
    while (1) {
        printf("Sleeping for 1 second...\n");
        sleep(1);
    }

    return 0;
}
