#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Առաջին հոսքի ֆունկցիա: կրկնապատկում է ստացված թիվը
void *thread1Func(void *arg) {
    int num = *(int *)arg;
    int *result = malloc(sizeof(int));
    if (!result) {
        perror("malloc");
        pthread_exit(NULL);
    }
    *result = num * 2;
    printf("Thread 1: %d * 2 = %d\n", num, *result);
    return result;
}

// Երկրորդ հոսքի ֆունկցիա: եռապատկում է ստացված թիվը
void *thread2Func(void *arg) {
    int num = *(int *)arg;
    int *result = malloc(sizeof(int));
    if (!result) {
        perror("malloc");
        pthread_exit(NULL);
    }
    *result = num * 3;
    printf("Thread 2: %d * 3 = %d\n", num, *result);
    return result;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int input = atoi(argv[1]);
    pthread_t t1, t2;
    int s;

    // Ստեղծում ենք առաջին հոսքը
    s = pthread_create(&t1, NULL, thread1Func, &input);
    if (s != 0) {
        fprintf(stderr, "pthread_create error\n");
        return EXIT_FAILURE;
    }

    // Սպասում ենք առաջին հոսքի ավարտին և ստանում արդյունքը
    int *res1;
    s = pthread_join(t1, (void **)&res1);
    if (s != 0 || !res1) {
        fprintf(stderr, "pthread_join error\n");
        return EXIT_FAILURE;
    }

    // Ստացված արժեքը փոխանցում ենք երկրորդ հոսքին
    s = pthread_create(&t2, NULL, thread2Func, res1);
    if (s != 0) {
        fprintf(stderr, "pthread_create error\n");
        free(res1);
        return EXIT_FAILURE;
    }

    int *res2;
    s = pthread_join(t2, (void **)&res2);
    if (s != 0 || !res2) {
        fprintf(stderr, "pthread_join error\n");
        free(res1);
        return EXIT_FAILURE;
    }

    // Արտածում ենք վերջնական արդյունքը
    printf("Final result in main: %d\n", *res2);

    // Ազատում հիշողությունը
    free(res1);
    free(res2);

    return EXIT_SUCCESS;
}
