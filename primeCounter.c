#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <stdatomic.h>

#include "queue.c"

// Function to check if a number is prime
bool isPrime(int n) {
    if (n <= 1) {
        return false;
    }
    if (n == 2 || n == 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }

    // Check divisors up to the square root of n
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

// func for the producerThread

void *inputNumbers(void *arg)
{
    Queue *q = (Queue *)arg;
    int num;

    while (scanf("%d", &num) != EOF)
    {
        enqueue(q, num);
    }
    for (int i = 6; i > 0; i--)
    {
        enqueue(q, -1);
    }

    return NULL;
}

void *outputNumbers(void *arg)
{
    Queue *q = (Queue *)arg;
    int num;

    while (1)
    {
        num = dequeue(q);
        if (isPrime(num))
        {
            atomic_fetch_add(&q->prime_counter, 1);
        }
        if (num == -1)
            break;
    }
    return NULL;
}

int main()
{

    int num;
    int total_counter = 0;
    Queue *queue = initializeQueue();

    pthread_t producerThread, consumerThread1, consumerThread2, consumerThread3, consumerThread4, consumerThread5, consumerThread6;

    pthread_create(&producerThread, NULL, inputNumbers, queue);
    pthread_create(&consumerThread1, NULL, outputNumbers, queue);
    pthread_create(&consumerThread2, NULL, outputNumbers, queue);
    pthread_create(&consumerThread3, NULL, outputNumbers, queue);
    pthread_create(&consumerThread4, NULL, outputNumbers, queue);
    pthread_create(&consumerThread5, NULL, outputNumbers, queue);
    pthread_create(&consumerThread6, NULL, outputNumbers, queue);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread1, NULL);
    pthread_join(consumerThread2, NULL);
    pthread_join(consumerThread3, NULL);
    pthread_join(consumerThread4, NULL);
    pthread_join(consumerThread5, NULL);
    pthread_join(consumerThread6, NULL);


    printf("%d total primes.\n", queue->prime_counter);

    return 0;
}