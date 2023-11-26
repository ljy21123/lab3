#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_SENTENCES 4
#define MAX_LENGTH 100

int main() {
    char sentences[MAX_SENTENCES][MAX_LENGTH] = {
        "동해물과 백두산이 마르고 닳도록",
        "하느님이 보우하사 우리나라 만세",
        "무궁화 삼천리 화려강산",
        "대한사람 대한으로 길이 보전하세",
    };

    int errors = 0;
    time_t start, end;
    double typingTime;

    printf("타자 연습\n-----------\n");
    start = time(NULL); // 타이핑 시작 시간 기록

    for (int i = 0; i < MAX_SENTENCES; i++) {
        printf("이 문장을 타이핑하세요: '%s'\n", sentences[i]);
        char input[MAX_LENGTH];
        fgets(input, MAX_LENGTH, stdin); // 사용자 입력 받기

        // 개행 문자 제거
        input[strcspn(input, "\n")] = 0;

        // 오류 수 계산
        if (strcmp(sentences[i], input) != 0) {
            errors++;
        }
    }

    end = time(NULL); // 타이핑 종료 시간 기록
    typingTime = difftime(end, start); // 총 타이핑 시간 계산 (초 단위)

    // 분 단위로 변환 후 분당 타자수 계산
    double wpm = (MAX_SENTENCES / typingTime) * 60;

    printf("테스트 완료.\n");
    printf("총 오타 수: %d\n", errors);
    printf("평균 분당 타자수: %.2f\n", wpm);

    return 0;
}
