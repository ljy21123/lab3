#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

#define QUEUE_NAME  "/chat_queue"  // 메시지 큐의 이름 정의
#define MAX_SIZE    1024           // 수신할 메시지의 최대 크기 정의
#define MSG_STOP    "exit"         // 세션을 종료하기 위한 메시지

int main() {
    mqd_t mq; // 메시지 큐 디스크립터
    char buffer[MAX_SIZE]; // 수신 메시지를 저장할 버퍼

    // 메시지 큐 열기 (읽기 전용)
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, NULL);
    if (mq == -1) {
        perror("mq_open"); // 메시지 큐 열기 실패 시 오류 메시지 출력
        exit(1);
    }

    printf("Waiting for messages...\n");
    while (1) {
        ssize_t bytes_read;

        // 메시지 큐에서 메시지 수신
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        if (bytes_read >= 0) {
            printf("Received: %s", buffer); // 수신된 메시지 출력
            // 'exit' 메시지를 받으면 반복문 탈출
            if (!strncmp(buffer, MSG_STOP, strlen(MSG_STOP))) {
                break;
            }
        } else {
            perror("mq_receive"); // 메시지 수신 실패 시 오류 메시지 출력
        }
    }

    mq_close(mq); // 메시지 큐 닫기
    mq_unlink(QUEUE_NAME); // 메시지 큐 삭제
    return 0;
}
