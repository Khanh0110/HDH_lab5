#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

int x = 0;                // Biến chia sẻ
pthread_mutex_t mutex;    // Mutex để đồng bộ hóa

void* processA(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex); // Bắt đầu vùng quan trọng
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("Process A: x = %d\n", x);
        pthread_mutex_unlock(&mutex); // Kết thúc vùng quan trọng
        usleep(100000); // Tạm nghỉ 100ms để dễ quan sát
    }
    return NULL;
}

void* processB(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex); // Bắt đầu vùng quan trọng
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("Process B: x = %d\n", x);
        pthread_mutex_unlock(&mutex); // Kết thúc vùng quan trọng
        usleep(100000); // Tạm nghỉ 100ms để dễ quan sát
    }
    return NULL;
}

int main() {
    pthread_t threadA, threadB;

    // Khởi tạo mutex
    pthread_mutex_init(&mutex, NULL);

    // Tạo 2 thread
    pthread_create(&threadA, NULL, processA, NULL);
    pthread_create(&threadB, NULL, processB, NULL);

    // Chờ các thread kết thúc
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);

    // Hủy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
