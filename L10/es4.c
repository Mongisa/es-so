#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
	char str[MAX_SIZE];
	int cont;
	pthread_mutex_t mutex;
	pthread_cond_t non_pieno;
	pthread_cond_t non_vuoto;
} TParam;

void* producer (void*);

int main() {
	TParam p;
	pthread_t tid;

	p.cont = 0;

	pthread_mutex_init(&p.mutex,NULL);
	pthread_cond_init(&p.non_pieno,NULL);
	pthread_cond_init(&p.non_vuoto,NULL);

	if(pthread_create(&tid,NULL,producer,&p) != 0) {
		printf("pthread_create err\n");
		return 2;
	}

	while(true) {
		char nuovo[MAX_SIZE];

		pthread_mutex_lock(&p.mutex);
		while(p.cont==0)
			pthread_cond_wait(&p.non_vuoto,&p.mutex);
		strcpy(nuovo,p.str);
		p.cont--;
		pthread_mutex_unlock(&p.mutex);
		pthread_cond_signal(&p.non_pieno);

		printf("Lunghezza stringa: %d\n",(int)strlen(nuovo));
	}

	return 0;
}

void* producer (void* arg) {
	TParam* p = (TParam *)arg;

	while(true) {
		pthread_mutex_lock(&p->mutex);
		while(p->cont==1)
			pthread_cond_wait(&p->non_pieno,&p->mutex);
		printf("Inserisci stringa: ");
		fgets(p->str, sizeof(p->str), stdin);
		p->cont++;
		pthread_mutex_unlock(&p->mutex);
		pthread_cond_signal(&p->non_vuoto);
	}
}
