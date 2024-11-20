#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Shared data
int shared_data = 0;

// Synchronization primitives
sem_t rw_mutex;  // Semaphore to control access to shared data (writers' priority)
sem_t mutex;     // Semaphore to protect the `reader_count`
int reader_count = 0;  // Count of active readers

void* reader(void* arg) {
    int reader_id = *((int*)arg);
    for (int i = 0; i < 3; i++) {  // Each reader reads 3 times
        sem_wait(&mutex);
        reader_count++;
        if (reader_count == 1) {
            sem_wait(&rw_mutex);  // First reader locks the resource for readers
        }
        sem_post(&mutex);

        // Reading shared data
        printf("Reader %d: read shared data = %d\n", reader_id, shared_data);
        sleep(1);  // Simulate reading time

        sem_wait(&mutex);
        reader_count--;
        if (reader_count == 0) {
            sem_post(&rw_mutex);  // Last reader unlocks the resource
        }
        sem_post(&mutex);

        sleep(2);  // Simulate time between reads
    }
    return NULL;
}

void* writer(void* arg) {
    int writer_id = *((int*)arg);
    for (int i = 0; i < 3; i++) {  // Each writer writes 3 times
        sem_wait(&rw_mutex);  // Writers lock the resource

        // Writing shared data
        shared_data += 10;
        printf("Writer %d: updated shared data to %d\n", writer_id, shared_data);
        sleep(2);  // Simulate writing time

        sem_post(&rw_mutex);  // Writers unlock the resource
        sleep(3);  // Simulate time between writes
    }
    return NULL;
}

int main() {
    pthread_t readers[5], writers[2];
    int reader_ids[5], writer_ids[2];

    // Initialize semaphores
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);

    // Create reader threads
    for (int i = 0; i < 5; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Create writer threads
    for (int i = 0; i < 2; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Wait for all threads to complete
    for (int i = 0; i < 5; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < 2; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);

    return 0;
}
