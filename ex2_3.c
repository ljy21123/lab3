#include <sys/stat.h>
#include <stdio.h>

int main() {
    char dirname[100];

    // 사용자로부터 생성할 디렉토리 이름 입력 받기
    printf("생성할 폴더 이름: ");
    scanf("%99s", dirname);

    // 디렉토리 생성
    if (mkdir(dirname, 0777) == -1) {
        perror("mkdir");
        return -1;
    }
    printf("폴더 생성 완료!\n");
    return 0;
}
