#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>

int sum; // Global variable

// Function prototype for thread execution
void *compute_sum(void *arg);

int main(int argc, char *argv[]) {
    if (argc!=2) {
        printf("Insert an integer as argument!\n");
        return EXIT_FAILURE;
    }
    int number = atoi(argv[1]);

    pthread_t tid; // thread id
    if (pthread_create(&tid, NULL, compute_sum, &number) != 0) {
        printf("Thread creation error!\n");
        return 2;
    }

    // wait for thread
    pthread_join(tid, NULL);

    printf("Sum = %d\n", sum);
    return EXIT_SUCCESS;
}

void *compute_sum(void *arg) {
    int *pnumber = (int *)arg;
    int i, n=*pnumber;
    sum=0;
    for(i=1; i <= n; i++)
        sum += i;
    return NULL;
}
