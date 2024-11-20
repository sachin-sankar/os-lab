
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

sem_t chopsticks[NUM_PHILOSOPHERS];  // Semaphores for chopsticks

void* philosopher(void* arg) {
    int id = *((int*)arg);

    for (int i = 0; i < 3; i++) {  // Each philosopher eats 3 times
        printf("Philosopher %d is thinking...\n", id);
        sleep(1);  // Simulate thinking time

        // Pick up chopsticks (ensure no deadlock with ordered locking)
        sem_wait(&chopsticks[id]);              // Pick up left chopstick
        sem_wait(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]);  // Pick up right chopstick

        // Eat
        printf("Philosopher %d is eating...\n", id);
        sleep(1);  // Simulate eating time

        // Put down chopsticks
        sem_post(&chopsticks[id]);              // Put down left chopstick
        sem_post(&chopsticks[(id + 1) % NUM_PHILOSOPHERS]);  // Put down right chopstick

        printf("Philosopher %d finished eating and is thinking again.\n", id);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    int ids[NUM_PHILOSOPHERS];

    // Initialize semaphores for chopsticks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}
