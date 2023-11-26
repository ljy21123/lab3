#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

void listDirectory(const char *basePath) {
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir) {
        return;
    }

    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            printf("%s/%s\n", basePath, dp->d_name);

            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            struct stat path_stat;
            stat(path, &path_stat);
            if (S_ISDIR(path_stat.st_mode)) {
                listDirectory(path);
            }
        }
    }

    closedir(dir);
}

int main() {
    char cwd[1024];

    // 현재 작업 디렉토리를 가져옴
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        listDirectory(cwd); // 현재 디렉토리부터 탐색 시작
    } else {
        perror("getcwd");
        return -1;
    }

    return 0;
}
