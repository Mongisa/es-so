#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

bool primo(int n) {
	if(n%2 == 0) {
		return n==2;
	}
	int i;
	for(i=3;i*i<=n; i+=2) {
		if(n%i == 0) return false;
	}
	return true;
}

void genera_risultato(int k, int *n1, int *n2) {
	int i = k+1;
	while(!primo(i))
		i++;
	*n1 = i;
	i++;
	while(!primo(i))
		i++;
	*n2 = i;
}

int main(int argc, char *argv[]) {
	int fd[2], num[2];

	if(argc != 2) {
		printf("Inserisci un numero!\n");
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);

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

		genera_risultato(n,&num[0],&num[1]);

		for(int i=0;i<2;i++) {
			int sent = write(fd[1], &num[i], sizeof(int));

			if(sent < sizeof(int)) {
				printf("Sending error!\n");
				return EXIT_FAILURE;
			}
		}
		close(fd[1]);
	} else {
		//Parent process
		close(fd[1]);

		for(int i=0;i<2;i++) {
			int received = read(fd[0], &num[i], sizeof(int));

			if(received < sizeof(int)) {
				printf("Receiving error!\n");
				return EXIT_FAILURE;
			}
		}

		printf("Numeri ricevuti: %d %d\n", num[0], num[1]);

		close(fd[0]);
	}
	
	return EXIT_SUCCESS;
}
