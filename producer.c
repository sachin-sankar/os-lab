#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5  // Buffer size

int buffer[BUFFER_SIZE];  // Shared buffer
int in = 0, out = 0;      // Buffer pointers

sem_t empty;  // Tracks empty slots
sem_t full;   // Tracks filled slots
sem_t mutex;  // Binary semaphore for mutual exclusion

void* producer(void* arg) {
    for (int i = 1; i <= 10; i++) {  // Produce 10 items
        sem_wait(&empty);  // Wait for an empty slot
        sem_wait(&mutex);  // Lock critical section

        buffer[in] = i;  // Produce item
        printf("Produced: %d\n", i);
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);  // Unlock critical section
        sem_post(&full);   // Signal a filled slot

        sleep(1);  // Simulate production time
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 1; i <= 10; i++) {  // Consume 10 items
        sem_wait(&full);  // Wait for a filled slot
        sem_wait(&mutex); // Lock critical section

        int item = buffer[out];  // Consume item
        printf("Consumed: %d\n", item);
        out = (out + 1) % BUFFER_SIZE;

        sem_post(&mutex);  // Unlock critical section
        sem_post(&empty);  // Signal an empty slot

        sleep(2);  // Simulate consumption time
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Initialize semaphores
    sem_init(&empty, 0, BUFFER_SIZE);  // BUFFER_SIZE empty slots
    sem_init(&full, 0, 0);             // 0 filled slots initially
    sem_init(&mutex, 0, 1);            // Binary semaphore, initially unlocked

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    // Destroy semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}

