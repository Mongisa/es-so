#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define START1 1
#define END1 50
#define START2 51
#define END2 100

typedef struct {
	int start; //Inizio intervallo da sommare
	int end; //Fine dell'intervallo da sommare
	int *psomma; //Puntatore alla variabile somma
	pthread_mutex_t *mutex; //Mutex per la sincronizzazione
} TParam;

void *calc_sum(void*);

int main(int argc, char *argv[]) {

	int somma = 0;
	pthread_mutex_t mutex;

	pthread_t tid[2];

	TParam arg[2];

	if(pthread_mutex_init(&mutex, NULL) != 0) {
		printf("Mutex err\n");
		return 1;
	}

	arg[0].start = START1;
	arg[0].end = END1;
	arg[0].psomma = &somma;
	arg[0].mutex = &mutex;

	if(pthread_create(&tid[0], NULL, calc_sum,&arg[0]) != 0) {
		printf("Pthread_create err\n");
		return 1;
	}

	arg[1].start = START2;
	arg[1].end = END2;
	arg[1].psomma = &somma;
	arg[1].mutex = &mutex;

	if(pthread_create(&tid[1], NULL, calc_sum, &arg[1]) != 0) {
		printf("Pthread_create err\n");
		return 1;
	}

	for(int i=0;i<2;i++)
		pthread_join(tid[i],NULL);

	printf("Somma: %d\n",somma);
	return 0;
}

void *calc_sum(void *arg) {
	TParam *d = (TParam*)arg;

	int temp = 0;

	for(int i=d->start; i<=d->end; i++) {
		temp += i;
	}

	pthread_mutex_lock(d->mutex);
	*(d->psomma) += temp;
	pthread_mutex_unlock(d->mutex);
}
