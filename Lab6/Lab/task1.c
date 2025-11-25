// thread_return_length.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *threadFunc(void *arg) {
    const char *s = (const char *)arg;
    size_t len = strlen(s);

    // Հաստատենք տեղ հիշողություն, որ վերադարձնենք
    size_t *pret = malloc(sizeof(size_t));
    if (pret == NULL) {
        perror("malloc");
        // պարտադիր առնվազն վերադարձնել ինչ-որ բան՝ NULL նշանակում error
        return NULL;
    }
    *pret = len;
    return pret; // գլխավոր հոսքը կստանա այս pointer-ը pthread_join-ով
}

int main(void) {
    pthread_t tid;
    const char *message = "Hello from thread!";
    int s;

    s = pthread_create(&tid, NULL, threadFunc, (void *)message);
    if (s != 0) {
        fprintf(stderr, "pthread_create error: %s\n", strerror(s));
        return EXIT_FAILURE;
    }

    // Ինձ թվում է main()-ը կարող է շարունակել ինչ-որ բան անել,
    // բայց այս տիպիկ առաջադրանքի համար սպասում ենք հոսքի ավարտին:
    void *retptr;
    s = pthread_join(tid, &retptr);
    if (s != 0) {
        fprintf(stderr, "pthread_join error: %s\n", strerror(s));
        return EXIT_FAILURE;
    }

    if (retptr == NULL) {
        fprintf(stderr, "Thread returned NULL (error)\n");
        return EXIT_FAILURE;
    }

    size_t len = *(size_t *)retptr;
    printf("Thread returned length = %zu\n", len);

    free(retptr); // ազատում ենք այն հիշողությունը որ հոսքը malloc արեց
    return EXIT_SUCCESS;
}
