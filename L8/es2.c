#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int p;
	long long int res;
} dataTy;

// Function prototype for thread execution
void *factorial_thread(void *arg);
long long int factorial(int);

int main(int argc, char *argv[]) {
    dataTy d[3];

    do {
    	printf("Inserisci 1 numero: ");
	scanf("%d",&d[0].p);
    } while(d[0].p<=0);

    do {
    	printf("Inserisci 2 numero: ");
	scanf("%d",&d[1].p);
    } while(d[1].p<0 || d[1].p>d[0].p);

    d[2].p = d[0].p - d[1].p;

    pthread_t tid[3]; // thread id

    for(int i=0;i<3;i++) {
    	if(pthread_create(&tid[i], NULL, factorial_thread, &d[i]) != 0) {
		printf("Thread creation error\n");
		return 2;
	}
    }

    for(int i=0;i<3;i++) {
    	pthread_join(tid[i], NULL);
    }

    printf("%d!: %lld, %d!: %lld, (%d-%d)!: %lld\n", d[1].p ,d[1].res, d[0].p, d[0].res, d[0].p,d[1].p,d[2].res);

    float res = (float)d[0].res/(d[1].res*d[2].res);

    printf("Coefficiente binomiale: %.2f\n",res);

    return EXIT_SUCCESS;
}

void *factorial_thread (void *arg) {
	dataTy* d = (dataTy*)arg;
	d->res = factorial(d->p);	
}

long long int factorial(int n) {
	return (n==0 || n==1) ? 1 : factorial(n-1)*n;
}
