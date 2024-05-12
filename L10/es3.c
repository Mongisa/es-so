#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define DELAY 3

typedef struct {
	int conteggio;
	pthread_mutex_t mutex;
	pthread_cond_t max;
	pthread_cond_t min;
} TParam;

void* counter(void*);
void* print_and_reset(void*);

int main() {
	pthread_t tid[2];
	
	TParam p;

	p.conteggio=0;

	pthread_mutex_init(&p.mutex,NULL);
	pthread_cond_init(&p.max,NULL);
	pthread_cond_init(&p.min,NULL);

	//Init counter thread
	if(pthread_create(&tid[0],NULL,counter,&p) != 0) {
		printf("pthread_create err\n");
		return 2;
	}

	//Init print and reset thread
	if(pthread_create(&tid[1],NULL,print_and_reset,&p) != 0) {
		printf("pthread_create err\n");
		return 2;
	}

	for(int i=0;i<2;i++) {
		pthread_join(tid[i],NULL);
	} 
	
	return 0;
}

void* counter(void* arg) {
	TParam *p = (TParam *) arg;
	
	while(true) {
		pthread_mutex_lock(&p->mutex);
		while(p->conteggio > 10) {
			pthread_cond_signal(&p->max);
			pthread_cond_wait(&p->min,&p->mutex);
		}
		printf("COUNTER=%d\n",p->conteggio);
		p->conteggio++;
		pthread_mutex_unlock(&p->mutex);
		sleep(DELAY);
	}
}

void* print_and_reset(void* arg) {
	TParam *p = (TParam *) arg;

	while(true) {
		pthread_mutex_lock(&p->mutex);
		while(p->conteggio <= 10) {
			pthread_cond_wait(&p->max,&p->mutex);
		}

		printf("CONTATORE RIPORTATO A 0\n");
		p->conteggio = 0;
		pthread_mutex_unlock(&p->mutex);
		pthread_cond_signal(&p->min);
	}
}
