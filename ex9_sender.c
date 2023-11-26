#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define QUEUE_NAME  "/chat_queue"  // 메시지 큐 이름 정의
#define MAX_SIZE    1024           // 메시지 최대 크기 정의
#define MSG_STOP    "exit"         // 세션 종료 메시지

int main() {
    mqd_t mq; // 메시지 큐 디스크립터
    char buffer[MAX_SIZE]; // 메시지 버퍼

    struct mq_attr attr; // 메시지 큐 속성 구조체

    // 메시지 큐 속성 설정
    attr.mq_flags = 0;         // 기본 플래그
    attr.mq_maxmsg = 10;       // 큐에 저장할 수 있는 최대 메시지 수
    attr.mq_msgsize = MAX_SIZE; // 각 메시지의 최대 크기
    attr.mq_curmsgs = 0;       // 현재 큐에 있는 메시지 수

    // 메시지 큐 생성 (쓰기 전용으로 열기)
    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq == -1) {
        perror("mq_open"); // 메시지 큐 열기 실패 시 오류 메시지 출력
        exit(1);
    }

    printf("Enter 'exit' to end the session\n");
    do {
        printf("> ");
        fflush(stdout); // 표준 출력 버퍼를 강제로 비움
        memset(buffer, 0, MAX_SIZE); // 버퍼 초기화
        fgets(buffer, MAX_SIZE, stdin); // 사용자 입력 받기

        // 메시지 큐로 메시지 보내기
        if (mq_send(mq, buffer, MAX_SIZE, 0) == -1) {
            perror("mq_send"); // 메시지 전송 실패 시 오류 메시지 출력
            continue;
        }
    } while (strncmp(buffer, MSG_STOP, strlen(MSG_STOP))); // 'exit' 입력 시 종료

    mq_close(mq); // 메시지 큐 닫기
    mq_unlink(QUEUE_NAME); // 메시지 큐 삭제
    return 0;
}
