#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define BUFF_SIZE 4
#define MAIN_SLEEP 60

#define PROD_THREADS 1
#define CONS_THREADS 2

typedef struct {
	int elem[BUFF_SIZE];
	int primo;
	int ultimo;
	int cont;
	pthread_mutex_t mutex; //Mutex per la sincronizzazione
	pthread_cond_t non_pieno;
	pthread_cond_t non_vuoto;
} TBuffer;

typedef struct {
	TBuffer *buffer;
	int id;
	int ritardo;
} TParam;

void *producer(void*);
void *consumer(void*);

int main(int argc, char *argv[]) {

	pthread_mutex_t mutex;
	pthread_cond_t non_pieno, non_vuoto;

	pthread_t tid_p[PROD_THREADS];
	pthread_t tid_c[CONS_THREADS];

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&non_pieno, NULL);
	pthread_cond_init(&non_vuoto, NULL);

	TBuffer b;

	TParam prod[PROD_THREADS];
	TParam cons[CONS_THREADS];

	b.primo = 0;
	b.ultimo = 0;
	b.cont = 0;
	b.mutex = mutex;
	b.non_pieno = non_pieno;
	b.non_vuoto = non_vuoto;

	for(int i=0;i<PROD_THREADS;i++) {
		prod[i].buffer = &b;
		prod[i].id = i;
		prod[i].ritardo = 2*(i+1);
	}

	for(int i=0;i<CONS_THREADS; i++) {
		cons[i].buffer = &b;
		cons[i].id = 50+i;
		cons[i].ritardo = 3*(i+1); 
	}

	//Creazione produttori
	for(int i=0;i<PROD_THREADS;i++) {
		if(pthread_create(&tid_p[i], NULL, producer,&prod[i]) != 0) {
			printf("Pthread_create err\n");
			return 1;
		}
	}

	//Creazione consumatori
	for(int i=0; i<CONS_THREADS; i++) {
		if(pthread_create(&tid_c[i], NULL, consumer, &cons[i]) != 0) {
			printf("Pthread_create err\n");
			return 1;
		}
	}

	sleep(MAIN_SLEEP);

	return 0;
}

void *producer (void *arg) {
	TParam *p = (TParam *) arg;
	int nuovo;
	int i = 1;
	while(true) {
		sleep(p->ritardo);
		nuovo = i++;
		pthread_mutex_lock(&p->buffer->mutex);
		while(p->buffer->cont==BUFF_SIZE)
			pthread_cond_wait(&p->buffer->non_pieno, &p->buffer->mutex);
		p->buffer->elem[p->buffer->ultimo] = nuovo;
		p->buffer->ultimo = (p->buffer->ultimo + 1) % BUFF_SIZE;
		p->buffer->cont++;
		pthread_cond_signal(&p->buffer->non_vuoto);
		pthread_mutex_unlock(&p->buffer->mutex);

		printf("PRODUCER ID[%d] DATO=%d\n",p->id,nuovo);
	}
}

void *consumer (void *arg) {
	TParam *p = (TParam *) arg;
	int val;
	while(true) {
		sleep(p->ritardo);
		pthread_mutex_lock(&p->buffer->mutex);
		while(p->buffer->cont == 0)
			pthread_cond_wait(&p->buffer->non_vuoto, &p->buffer->mutex);
		val = p->buffer->elem[p->buffer->primo];
		p->buffer->primo = (p->buffer->primo + 1) % BUFF_SIZE;
		p->buffer->cont--;
		pthread_cond_signal(&p->buffer->non_pieno);
		pthread_mutex_unlock(&p->buffer->mutex);

		printf("CONSUMER ID[%d] DATO=%d\n",p->id,val);
	}
}
