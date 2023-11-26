#include <stdio.h>

int main() {
    char filename[100], content[1000];

    // 사용자로부터 파일 이름 입력 받기
    printf("생성할 파일 이름 입력: ");
    scanf("%99s", filename);

    // 파일 열기 (쓰기 모드)
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("fopen");
        return -1;
    }

    // 사용자로부터 파일 내용 입력 받기
    printf("파일 내용 입력: ");
    scanf(" %[^\n]", content); // 뉴라인 문자 전까지 모든 문자를 받음

    // 파일에 내용 쓰기
    fprintf(file, "%s\n", content);
    fclose(file); // 파일 닫기
    return 0;
}
