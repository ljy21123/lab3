#include <stdio.h>

int main() {
    char filename[100];

    // 사용자로부터 읽을 파일 이름 입력 받기
    printf("읽을 파일 이름: ");
    scanf("%99s", filename);

    // 파일 열기 (읽기 모드)
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    char line[100];
    // 파일에서 한 줄씩 읽어 출력
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("%s", line);
    }

    fclose(file); // 파일 닫기
    return 0;
}
