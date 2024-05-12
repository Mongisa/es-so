#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define THREAD_PROD 1
#define THREAD_CONS 2
#define MAIN_SLEEP 60
#define SIZE 4

void* producer(void*);
void* consumer(void*);

typedef struct {
	int buff[SIZE];
	int start;
	int end;
	int count;
	pthread_mutex_t mutex;
	pthread_cond_t non_pieno;
	pthread_cond_t non_vuoto;
} TBuffer;

typedef struct {
	TBuffer *buffer;
	int id;
	int delay;
} TParam;

int main() {
	TBuffer buffer;

	pthread_mutex_init(&buffer.mutex,NULL);
	pthread_cond_init(&buffer.non_pieno,NULL);
	pthread_cond_init(&buffer.non_vuoto,NULL);

	TParam prod[THREAD_PROD];
	TParam cons[THREAD_CONS];

	pthread_t prod_tid[THREAD_PROD];
	pthread_t cons_tid[THREAD_CONS];

	for(int i=0;i<THREAD_PROD;i++) {
		prod[i].buffer = &buffer;
		prod[i].id = i;
		prod[i].delay = 2*(i+1);
	}

	for(int i=0;i<THREAD_CONS;i++) {
		cons[i].buffer = &buffer;
		cons[i].id = THREAD_PROD+i;
		cons[i].delay = 3*(i+1);
	}

	buffer.start = 0;
	buffer.end = 0;
	buffer.count = 0;

	for(int i=0;i<THREAD_PROD;i++) {
		if(pthread_create(&prod_tid[i],NULL,producer,&prod[i]) != 0) {
			printf("pthread_create err\n");
			return 1;
		}
	}

	for(int i=0;i<THREAD_CONS;i++) {
		if(pthread_create(&cons_tid[i],NULL,consumer,&cons[i]) != 0) {
			printf("pthread_create err\n");
			return 1;
		}
	}

	sleep(MAIN_SLEEP);
	return 0;
}

void* producer(void* arg) {
	TParam* p = (TParam*) arg;
	int nuovo = 1;
	TBuffer *buffer = p->buffer;

	while(true) {
		sleep(p->delay);
		pthread_mutex_lock(&buffer->mutex);
		while(buffer->count==SIZE)
			pthread_cond_wait(&buffer->non_pieno,&buffer->mutex);
		buffer->buff[buffer->end] = nuovo;
		printf("PRODUCER ID=%d DATO=%d\n",p->id,nuovo);
		nuovo++;
		buffer->count++;
		buffer->end = (buffer->end + 1) % SIZE;
		pthread_mutex_unlock(&buffer->mutex);
		pthread_cond_signal(&buffer->non_vuoto);
	}
}

void* consumer(void* arg) {
	TParam* p = (TParam*) arg;
	TBuffer *buffer = p->buffer;
	int dato;
	while(true) {
		sleep(p->delay);
		pthread_mutex_lock(&buffer->mutex);
		while(buffer->count==0)
			pthread_cond_wait(&buffer->non_vuoto,&buffer->mutex);
		dato = buffer->buff[buffer->start];
	       	printf("CONSUMER ID=%d DATO=%d\n",p->id,dato);
		buffer->count--;
		buffer->start = (buffer->start + 1) % SIZE;
		pthread_mutex_unlock(&buffer->mutex);
		pthread_cond_signal(&buffer->non_pieno);	
	}
}
