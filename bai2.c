#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_SIZE 100  // Kích thước tối đa của mảng

int a[MAX_SIZE];       // Mảng toàn cục
int n = 0;             // Số lượng phần tử hiện có trong mảng
sem_t sem_add;         // Semaphore để kiểm soát thêm phần tử
sem_t sem_remove;      // Semaphore để kiểm soát lấy phần tử
pthread_mutex_t mutex; // Mutex để đồng bộ hóa truy cập mảng

// Thread 1: Thêm phần tử ngẫu nhiên vào mảng
void* producer(void* arg) {
    while (1) {
        int num = rand() % 100;  // Số ngẫu nhiên từ 0 đến 99

        sem_wait(&sem_add);  // Đảm bảo mảng chưa đầy
        pthread_mutex_lock(&mutex);  // Đồng bộ hóa truy cập mảng

        a[n++] = num;  // Thêm phần tử vào mảng
        printf("Produced: %d, Array size: %d\n", num, n);

        pthread_mutex_unlock(&mutex);  // Mở khóa mảng
        sem_post(&sem_remove);  // Cho phép lấy phần tử
        sleep(1);  // Tạm nghỉ để mô phỏng
    }
}

// Thread 2: Lấy phần tử từ mảng
void* consumer(void* arg) {
    while (1) {
        sem_wait(&sem_remove);  // Đảm bảo có phần tử để lấy
        pthread_mutex_lock(&mutex);  // Đồng bộ hóa truy cập mảng

        if (n > 0) {
            int index = rand() % n;  // Chọn phần tử ngẫu nhiên
            int removed = a[index];  // Lấy phần tử
            a[index] = a[--n];       // Dịch chuyển phần tử cuối cùng vào vị trí trống
            printf("Consumed: %d, Array size: %d\n", removed, n);
        } else {
            printf("Nothing in array a\n");
        }

        pthread_mutex_unlock(&mutex);  // Mở khóa mảng
        sem_post(&sem_add);  // Cho phép thêm phần tử
        sleep(1);  // Tạm nghỉ để mô phỏng
    }
}

int main() {
    srand(time(NULL));  // Khởi tạo seed cho số ngẫu nhiên

    pthread_t thread1, thread2;

    sem_init(&sem_add, 0, MAX_SIZE);   // Semaphore giới hạn số phần tử thêm
    sem_init(&sem_remove, 0, 0);       // Semaphore đảm bảo mảng không rỗng
    pthread_mutex_init(&mutex, NULL);  // Khởi tạo mutex

    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, consumer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    sem_destroy(&sem_add);
    sem_destroy(&sem_remove);
    pthread_mutex_destroy(&mutex);

    return 0;
}
