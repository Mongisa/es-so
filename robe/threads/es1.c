#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

int somma;

void *calcola_somma(void *arg);

int main(int argc, char* argv[]) {
	if(argc != 2) {
		printf("Nessun numero!\n");
		return 1;
	}

	int n=atoi(argv[1]);

	pthread_t tid;

	if(pthread_create(&tid, NULL, calcola_somma, &n) != 0) {
		printf("pthread_create error!\n");
		return 2;
	}

	pthread_join(tid, NULL);

	printf("Somma: %d\n", somma);
	return 0;
}

void *calcola_somma(void *arg) {
	int num = *((int *) arg);
	somma=0;
	for(int i=1;i<=num;i++)
		somma+=i;
	return NULL;
}
