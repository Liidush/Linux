#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

int main(int argc, char *argv[]) {
    if(argc != 2) return 1;
    int semid = atoi(argv[1]);

    struct sembuf op = {0, +1, 0};
    semop(semid, &op, 1);
    int val = semctl(semid, 0, GETVAL);
    printf("Incremented by 1. Current value: %d\n", val);
    return 0;
}
