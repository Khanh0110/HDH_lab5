#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int x = 0; // Biến chia sẻ giữa 2 process

void processA() {
    while (1) {
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("Process A: x = %d\n", x);
        usleep(100000); // Tạm nghỉ 100ms để dễ quan sát
    }
}

void processB() {
    while (1) {
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("Process B: x = %d\n", x);
        usleep(100000); // Tạm nghỉ 100ms để dễ quan sát
    }
}

int main() {
    pid_t pid = fork(); // Tạo process mới

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Process con - chạy processA
        processA();
    } else {
        // Process cha - chạy processB
        processB();
    }

    return 0;
}
