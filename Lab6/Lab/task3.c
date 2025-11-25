#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep()

// Detached հոսքի ֆունկցիա
void *detachedThreadFunc(void *arg) {
    const char *msg = (const char *)arg;
    printf("Detached thread: %s\n", msg);
    sleep(2); // թող հոսքը մի քիչ աշխատի
    printf("Detached thread finished work.\n");
    return NULL;
}

int main(void) {
    pthread_t tid;
    const char *message = "Hello from detached thread!";

    // Ստեղծում ենք հոսքը
    if (pthread_create(&tid, NULL, detachedThreadFunc, (void *)message) != 0) {
        perror("pthread_create");
        return EXIT_FAILURE;
    }

    // Դարձնում ենք detached
    if (pthread_detach(tid) != 0) {
        perror("pthread_detach");
        return EXIT_FAILURE;
    }

    printf("Main thread continues without waiting...\n");

    sleep(3); // main-ը մի քիչ սպասում է, որ detached հոսքը ավարտվի տեսնելու համար
    printf("Main thread finished.\n");

    return EXIT_SUCCESS;
}
