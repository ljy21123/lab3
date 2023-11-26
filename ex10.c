#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>

#define SHM_KEY 0x1234
#define SEM_KEY 0x5678
#define SHM_SIZE 1024

// 세마포어 유니온 정의
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

// 세마포어 초기화 함수
void init_sem(int sem_id, int value) {
    union semun sem_union;
    sem_union.val = value;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
        perror("semctl");
        exit(1);
    }
}

// 세마포어 대기 함수
void sem_wait(int sem_id) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; // P() 연산
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        perror("semop wait");
        exit(1);
    }
}

// 세마포어 신호 함수
void sem_signal(int sem_id) {
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; // V() 연산
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1) {
        perror("semop signal");
        exit(1);
    }
}

int main() {
    int shm_id, sem_id;
    void *shm_ptr;

    // 공유 메모리 생성
    shm_id = shmget(SHM_KEY, SHM_SIZE, 0644 | IPC_CREAT);
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    // 세마포어 생성
    sem_id = semget(SEM_KEY, 1, 0644 | IPC_CREAT);
    if (sem_id == -1) {
        perror("semget");
        exit(1);
    }

    // 공유 메모리에 attach
    shm_ptr = shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *) -1) {
        perror("shmat");
        exit(1);
    }

    // 세마포어 초기화
    init_sem(sem_id, 1);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // 자식 프로세스 - 쓰기
        int out_fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (out_fd == -1) {
            perror("open");
            exit(1);
        }

        while (1) {
            sem_wait(sem_id); // 세마포어 대기
            if (strlen((char *)shm_ptr) == 0) {
                sem_signal(sem_id);
                break;
            }
            write(out_fd, shm_ptr, strlen((char *)shm_ptr));
            memset(shm_ptr, 0, SHM_SIZE); // 공유 메모리 초기화
            sem_signal(sem_id); // 세마포어 신호
        }

        close(out_fd);
        exit(0);
    } else {
        // 부모 프로세스 - 읽기
        int in_fd = open("input.txt", O_RDONLY);
        if (in_fd == -1) {
            perror("open");
            exit(1);
        }

        char buffer[SHM_SIZE];
        ssize_t read_bytes;
        while ((read_bytes = read(in_fd, buffer, SHM_SIZE)) > 0) {
            sem_wait(sem_id); // 세마포어 대기
            memcpy(shm_ptr, buffer, read_bytes);
            sem_signal(sem_id); // 세마포어 신호
        }

        // 읽기 종료 신호
        sem_wait(sem_id);
        memset(shm_ptr, 0, SHM_SIZE);
        sem_signal(sem_id);

        close(in_fd);
        wait(NULL); // 자식 프로세스 종료 대기

        // 공유 메모리 detach 및 삭제
        shmdt(shm_ptr);
        shmctl(shm_id, IPC_RMID, NULL);

        // 세마포어 삭제
        union semun sem_union;
        if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
            perror("semctl");
            exit(1);
        }
    }

    return 0;
}
