#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MSSV_LAST4 717  

sem_t sem_sell;       
sem_t sem_produce;  
int sells = 0;       
int products = 0;     

void* processA(void* arg) {
    while (1) {
        sem_wait(&sem_sell);  
        sells++;              
        printf("Sold: %d, Available: %d\n", sells, products);
        sem_post(&sem_produce);  
    }
}
void* processB(void* arg) {
    while (1) {
        sem_wait(&sem_produce);  
        products++;              
        printf("Produced: %d\n", products);
        sem_post(&sem_sell);     
    }
}
int main() {
    pthread_t threadA, threadB;
    // Khởi tạo semaphore
    sem_init(&sem_sell, 0, 0);                 
    sem_init(&sem_produce, 0, MSSV_LAST4);     // Tạo tối đa 717 sản phẩm trong kho
    // Tạo hai tiến trình
    pthread_create(&threadA, NULL, processA, NULL);
    pthread_create(&threadB, NULL, processB, NULL);
    // Chờ các tiến trình hoàn thành
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    // Hủy semaphore
    sem_destroy(&sem_sell);
    sem_destroy(&sem_produce);

    return 0;
}
