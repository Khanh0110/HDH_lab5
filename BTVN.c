#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int x1 = 2, x2 = 3, x3 = 4, x4 = 5, x5 = 6, x6 = 7;
int w, v, y, z, ans;

sem_t sem_b, sem_c, sem_d, sem_e, sem_f, sem_g;

void* calculate_a(void* arg) {
    w = x1 * x2; // (a)
    printf("w = %d\n", w);
    sem_post(&sem_c); // Cho phép (e) thực hiện
    sem_post(&sem_d); // Cho phép (f) thực hiện
    return NULL;
}

void* calculate_b(void* arg) {
    v = x3 * x4; // (b)
    printf("v = %d\n", v);
    sem_post(&sem_c); // Cho phép (c) thực hiện
    sem_post(&sem_d); // Cho phép (d) thực hiện
    return NULL;
}

void* calculate_c(void* arg) {
    sem_wait(&sem_c); // Chờ (b) hoàn tất
    y = v * x5; // (c)
    printf("y (partial) = %d\n", y);
    sem_post(&sem_e); // Cho phép (e) thực hiện
    return NULL;
}

void* calculate_d(void* arg) {
    sem_wait(&sem_d); // Chờ (b) hoàn tất
    z = v * x6; // (d)
    printf("z (partial) = %d\n", z);
    sem_post(&sem_f); // Cho phép (f) thực hiện
    return NULL;
}

void* calculate_e(void* arg) {
    sem_wait(&sem_c); // Chờ (a) hoàn tất
    sem_wait(&sem_e); // Chờ (c) hoàn tất
    y = w * y; // (e)
    printf("y = %d\n", y);
    sem_post(&sem_g); // Cho phép (g) thực hiện
    return NULL;
}

void* calculate_f(void* arg) {
    sem_wait(&sem_d); // Chờ (a) hoàn tất
    sem_wait(&sem_f); // Chờ (d) hoàn tất
    z = w * z; // (f)
    printf("z = %d\n", z);
    sem_post(&sem_g); // Cho phép (g) thực hiện
    return NULL;
}

void* calculate_g(void* arg) {
    sem_wait(&sem_g); // Chờ (e) hoàn tất
    sem_wait(&sem_g); // Chờ (f) hoàn tất
    ans = y + z; // (g)
    printf("ans = %d\n", ans);
    return NULL;
}

int main() {
    pthread_t thread_a, thread_b, thread_c, thread_d, thread_e, thread_f, thread_g;

    // Khởi tạo các semaphore
    sem_init(&sem_c, 0, 0);
    sem_init(&sem_d, 0, 0);
    sem_init(&sem_e, 0, 0);
    sem_init(&sem_f, 0, 0);
    sem_init(&sem_g, 0, 0);

    // Tạo các thread
    pthread_create(&thread_a, NULL, calculate_a, NULL);
    pthread_create(&thread_b, NULL, calculate_b, NULL);
    pthread_create(&thread_c, NULL, calculate_c, NULL);
    pthread_create(&thread_d, NULL, calculate_d, NULL);
    pthread_create(&thread_e, NULL, calculate_e, NULL);
    pthread_create(&thread_f, NULL, calculate_f, NULL);
    pthread_create(&thread_g, NULL, calculate_g, NULL);

    // Chờ các thread kết thúc
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    pthread_join(thread_c, NULL);
    pthread_join(thread_d, NULL);
    pthread_join(thread_e, NULL);
    pthread_join(thread_f, NULL);
    pthread_join(thread_g, NULL);

    // Hủy semaphore
    sem_destroy(&sem_c);
    sem_destroy(&sem_d);
    sem_destroy(&sem_e);
    sem_destroy(&sem_f);
    sem_destroy(&sem_g);

    return 0;
}
