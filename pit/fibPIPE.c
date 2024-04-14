#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void fibonacci(int vet[],int n) {
	if(n <= 0) return;

	vet[0] = 0;
	if(n == 1) return;

	vet[1] = 1;
	if(n == 2) return;

	for(int i=2; i<n; i++) {
		vet[i] = vet[i-1] + vet[i-2];
	}
}

int main(int argc, char *argv[]) {
	int fd[2];

	if(argc != 2) {
		printf("Inserisci numero!\n");
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);

	int *v = (int*)malloc(n*sizeof(int));

	if(pipe(fd)<0) {
		printf("pipe error\n");
		return EXIT_FAILURE;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("fork error\n");
		return EXIT_FAILURE;
	}

	if(pid == 0) {
		//Child process
		close(fd[0]);

		fibonacci(v,n);

		int sent = write(fd[1],v,n*sizeof(int));

		if(sent<sizeof(v)) {
			printf("Errore in invio\n");
			return EXIT_FAILURE;
		}

		close(fd[1]);
	} else {
		//Father process
		close(fd[1]);

		int received = read(fd[0],v,n*sizeof(int));

		if(received<sizeof(v)) {
			printf("Errore in ricezione\n");
			return EXIT_FAILURE;
		}

		printf("Fibonacci: ");

		for(int i=0;i<n;i++) {
			printf("%d ",v[i]);
		}

		printf("\n");
		close(fd[0]);
	}

	free(v);
}
