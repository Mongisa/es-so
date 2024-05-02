#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

typedef struct {
	float* vet;
	int size;
} data;

void *calc_sqrt(void*);

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("No param!\n");
		return 1;
	}

	int size = atoi(argv[1]);

	float* vet = malloc(size*sizeof(float));

	if(vet == NULL) {
		printf("Malloc err\n");
		return 1;
	}

	for(int i=0; i<size; i++) {
		vet[i] = (float)(i+1);
	}

	pthread_t tid[2];

	data d[2];
	d[0].vet = vet;
	d[0].size = size/2;

	if(pthread_create(&tid[0], NULL, calc_sqrt,&d[0]) != 0) {
		printf("Pthread_create err\n");
		return 1;
	}

	d[1].vet = vet + (size/2);
	d[1].size = size - (size/2);

	if(pthread_create(&tid[1], NULL, calc_sqrt, &d[1]) != 0) {
		printf("Pthread_create err\n");
		return 1;
	}

	for(int i=0;i<2;i++)
		pthread_join(tid[i], NULL);

	printf("Ecco il vettore risultante: ");

	for(int i=0;i<size;i++) {
		printf("%.2f ",vet[i]);
	}

	printf("\n");

	free(vet);
	return 0;
}

void *calc_sqrt(void *v) {
	data *d = (data*)v;

	for(int i=0; i<d->size; i++) {
		d->vet[i] = sqrt(d->vet[i]);
	}	
}
