#include <stdio.h>
#include <dirent.h>

int main() {
    char dirname[100];

    // 사용자로부터 읽을 디렉토리 이름 입력 받기
    printf("읽을 폴더 이름: ");
    scanf("%99s", dirname);

    DIR *d;
    struct dirent *dir;

    // 디렉토리 열기
    d = opendir(dirname);
    if (d) {
        // 디렉토리 내의 모든 파일/디렉토리 이름 출력
        while ((dir = readdir(d)) != NULL) {
            printf("%s\n", dir->d_name);
        }
        closedir(d); // 디렉토리 닫기
    } else {
        perror("opendir");
        return -1;
    }
    return 0;
}
