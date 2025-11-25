#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10
int data[SIZE];

// Երկրորդ հոսքի ֆունկցիա՝ զույգ տարրերը լրացնում է
void *fillEven(void *arg) {
    for (int i = 1; i < SIZE; i += 2) {
        data[i] = i + 1; // զույգը 2, 4, 6...
    }
    return NULL;
}

int main(void) {
    pthread_t t1;
    int s;

    // Ստեղծում ենք երկրորդ հոսքը
    s = pthread_create(&t1, NULL, fillEven, NULL);
    if (s != 0) {
        fprintf(stderr, "pthread_create error\n");
        exit(EXIT_FAILURE);
    }

    // Գլխավոր հոսքը լրացնում է կենտ տարրերը
    for (int i = 0; i < SIZE; i += 2) {
        data[i] = i + 1; // 1, 3, 5, 7, 9
    }

    // Սպասում ենք երկրորդ հոսքի ավարտին
    s = pthread_join(t1, NULL);
    if (s != 0) {
        fprintf(stderr, "pthread_join error\n");
        exit(EXIT_FAILURE);
    }

    // Արտածում ենք լրացված զանգվածը
    printf("Array: ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    return 0;
}