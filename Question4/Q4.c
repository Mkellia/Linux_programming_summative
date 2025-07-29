#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_QUEUE_SIZE 10

int queue[MAX_QUEUE_SIZE];   // shared queue buffer
int count = 0;               // current number of items
int in = 0, out = 0;         // circular buffer indices

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

/*
 * Producer Thread
 * Produces an item every 2 seconds and adds it to the queue.
 * Waits if the queue is full.
 */
void* producer(void* arg) {
    int item = 1;

    while (1) {
        sleep(2); // simulate manufacturing time

        pthread_mutex_lock(&mutex);

        // Wait until there's space in the queue
        while (count == MAX_QUEUE_SIZE) {
            printf("Queue full. Producer waiting...\n");
            pthread_cond_wait(&not_full, &mutex);
        }

        // Produce item and add to queue
        queue[in] = item;
        in = (in + 1) % MAX_QUEUE_SIZE;
        count++;
        printf("Produced item %d | Queue size: %d\n", item, count);
        item++;

        // Signal the consumer that an item is available
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

/*
 * Consumer Thread
 * Consumes one item every 3 seconds from the queue.
 * Waits if the queue is empty.
 */
void* consumer(void* arg) {
    while (1) {
        sleep(3); // simulate packaging time

        pthread_mutex_lock(&mutex);

        // Wait until there's at least one item in the queue
        while (count == 0) {
            printf("Queue empty. Consumer waiting...\n");
            pthread_cond_wait(&not_empty, &mutex);
        }

        // Consume item from queue
        int item = queue[out];
        out = (out + 1) % MAX_QUEUE_SIZE;
        count--;
        printf("Consumed item %d | Queue size: %d\n", item, count);

        // Signal the producer that space is available
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Create producer and consumer threads
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);

    // Let the main thread wait for both (won’t actually end in this infinite sim)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);

    return 0;
}
