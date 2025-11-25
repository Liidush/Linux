#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "curr_time.h"

#define ARR_SIZE 5

static int arr[ARR_SIZE];
static int available = 0;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

// Հոսք, որը ավելացնում է available և ուղարկում ազդանշան
static void *threadFunc(void *arg)
{
    printf("Thread 1 starting at %s\n", currTime("%T"));

    pthread_mutex_lock(&threadMutex);
    available++;
    pthread_mutex_unlock(&threadMutex);

    // Ուղարկում ենք ազդանշան գլխավոր հոսքին
    pthread_cond_signal(&cond);

    sleep(2); // շարունակվում է իր աշխատանքը
    printf("Thread 1 terminating at %s\n", currTime("%T"));
    return NULL;
}

int main(void)
{
    printf("Main thread starting at %s\n", currTime("%T"));

    pthread_t t1;
    int s;

    // Ստեղծում ենք հոսքը
    s = pthread_create(&t1, NULL, threadFunc, NULL);
    if (s != 0) { fprintf(stderr, "pthread_create error\n"); exit(EXIT_FAILURE); }

    // Այստեղ չենք անում pthread_join
    // main thread-ը սպասում է ազդանշանին
    pthread_mutex_lock(&threadMutex);
    while (available == 0) { // safety loop
        pthread_cond_wait(&cond, &threadMutex);
    }
    available--; // սպառում ենք փոփոխականը
    pthread_mutex_unlock(&threadMutex);

    printf("Main thread received signal at %s\n", currTime("%T"));

    // Այժմ կարող ենք main-ին 3 վայրկյան քնել, որ տեսնենք hոսքի մնացած աշխատանքը
    sleep(3);
    printf("Main thread terminating at %s\n", currTime("%T"));

    exit(EXIT_SUCCESS);
}
