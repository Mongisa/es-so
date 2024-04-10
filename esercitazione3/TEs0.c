#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <pthread.h>

// Function prototype for thread execution
void *print_index(void *arg);

int main(int argc, char *argv[]) {
    if (argc!=2) {
        printf("Insert an integer as argument!\n");
        return EXIT_FAILURE;
    }
    int number = atoi(argv[1]);
    int arg[number];
    int i;

    pthread_t tid[number]; // array of thread id

    for(i=0;i<number;i++) {
	arg[i]=i;
    	if (pthread_create(&tid[i], NULL, print_index, &arg[i]) != 0) {
        	printf("Thread creation error!\n");
        	return 2;
    	}

	//pthread_join(tid[i],NULL);
    }
    for(i=0;i<number;i++) {
    		// wait for thread
    		pthread_join(tid[i], NULL);
    }
    return EXIT_SUCCESS;
}

void *print_index(void *arg) {
    int *index = (int *)arg;
    int n = *index;
    printf("Il mio indice: %d\n",n);
}
