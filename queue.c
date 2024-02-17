#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdatomic.h>


#define MAX_QUEUE_SIZE 1000 // Change this based on your requirements

typedef struct {
    int* arr;
    int front;
    int rear;
    int size;
    int capacity;
    atomic_int prime_counter;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} Queue;

Queue* initializeQueue() {
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    if (newQueue == NULL) {
        perror("Queue initialization");
        exit(EXIT_FAILURE);
    }

    newQueue->arr = (int*)malloc(MAX_QUEUE_SIZE * sizeof(int));
    if (newQueue->arr == NULL) {
        perror("Queue array allocation");
        free(newQueue);
        exit(EXIT_FAILURE);
    }

    newQueue->front = -1;
    newQueue->rear = -1;
    newQueue->size = 0;
    newQueue->capacity = MAX_QUEUE_SIZE;
    atomic_init(&newQueue->prime_counter, 0);

    pthread_mutex_init(&newQueue->mutex, NULL);
    pthread_cond_init(&newQueue->cond, NULL);

    return newQueue;
}

void enqueue(Queue* queue, int num) {
    pthread_mutex_lock(&queue->mutex);

    while ((queue->rear + 1) % queue->capacity == queue->front) {
        // Queue is full, wait until there is space
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    if (queue->front == -1) {
        // If the queue is initially empty
        queue->front = 0;
        queue->rear = 0;
    } else {
        // Move rear to the next position in a circular manner
        queue->rear = (queue->rear + 1) % queue->capacity;
    }

    // Insert the new element
    queue->arr[queue->rear] = num;
    queue->size++;

    pthread_mutex_unlock(&queue->mutex);
    pthread_cond_signal(&queue->cond); // Signal that a number has been added
}

int dequeue(Queue* queue) {
    pthread_mutex_lock(&queue->mutex);

    while (queue->size == 0) {
        // Queue is empty, wait until there is an element
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    int num = queue->arr[queue->front];
    if (queue->front == queue->rear) {
        // If there is only one element in the queue
        queue->front = -1;
        queue->rear = -1;
    } else {
        // Remove the front element in a circular manner
        queue->front = (queue->front + 1) % queue->capacity;
    }

    queue->size--;

    pthread_mutex_unlock(&queue->mutex);
    pthread_cond_signal(&queue->cond); // Signal that an element has been removed

    return num;
}

void removeQueue(Queue* queue) {
    free(queue->arr);
    free(queue);
}