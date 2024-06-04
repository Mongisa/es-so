#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *media(void *arg);
void *minimo(void *arg);
void *massimo(void *arg);

typedef struct {
	int* vet;
	int size;
} TArray;

int min, max;
float med;

int main() {

	int n;

	printf("Inserisci n: ");
	scanf("%d",&n);

	int *vet = malloc(n*sizeof(int));

	TArray s;

	s.vet = vet;
	s.size = n;


	for(int i=0;i<n;i++) {
		printf("Inserisci [%d]: ",i);
		scanf("%d",&vet[i]);
	}

	pthread_t tid[3];

	if(pthread_create(&tid[0], NULL, media, &s) != 0) {
		printf("pthread_create err\n");
		return 2;
	}

	if(pthread_create(&tid[1], NULL, minimo, &s) != 0) {
		printf("pthread_create err\n");
		return 2;
	}

	if(pthread_create(&tid[2], NULL, massimo, &s) != 0) {
		printf("pthread_create err\n");
		return 2;
	}

	for(int i=0;i<3;i++) {
		pthread_join(tid[i], NULL);
	}

	printf("Media: %.2f, Minimo: %d, Massimo: %d\n",med, min, max);

	free(vet);
}

void* media(void* arg) {
	TArray *p = (TArray *) arg;
	med=0;
	for(int i=0;i<p->size;i++) {
		med += (float)p->vet[i]/(float)p->size;
	}
	return NULL;
}

void* minimo(void* arg) {
	TArray *p = (TArray *) arg;
	min=p->vet[0];
	for(int i=1;i<p->size;i++) {
		if(p->vet[i] < min)
			min = p->vet[i];
	}

	return NULL;
}

void* massimo(void* arg) {
	TArray *p = (TArray *) arg;
	max=p->vet[0];
	for(int i=1;i<p->size;i++) {
		if(p->vet[i] > max)
			max = p->vet[i];
	}

	return NULL;
}
