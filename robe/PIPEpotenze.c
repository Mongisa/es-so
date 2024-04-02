#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/wait.h>

#define DIM 3
/* INSERIRE EVENTUALI FUNZIONI AUSILIARIE */
void sendInt(int fd,int *m);

int main(int argc, char *argv[]) {
	
	int fd[2];

	if(argc < 2) {
		printf("Nessun parametro fornito!\n");
		return 1;
	}

	//Converto il parametro in intero
	int n = atoi(argv[1]);

	if(pipe(fd)<0) {
		printf("Pipe failed!\n");
		return 1;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("Fork failed!\n");
		return 1;
	} else if(pid == 0) {
		//Child process
		close(fd[0]);

		int quadrato = n*n;

		sendInt(fd[1],&quadrato);

		int cubo = n*n*n;

		sendInt(fd[1],&cubo);

		int quarto = n*n*n*n;

		sendInt(fd[1],&quarto);

		close(fd[1]);
	} else {
		//Father process
		close(fd[1]);

		int messaggio[DIM];

		int received = read(fd[0],&messaggio,sizeof(messaggio));

		if(received < sizeof(messaggio)) {
			printf("Partial message!\n");
			return 1;
		}

		printf("Quadrato: %d\n",messaggio[0]);
		printf("Cubo: %d\n",messaggio[1]);
		printf("Quarto: %d\n",messaggio[2]);

		close(fd[0]);
	}

	return 0;
}

void sendInt (int fd, int *m) {
	int sent = write(fd,m,sizeof(int));

	if(sent < sizeof(int)) {
		printf("Sending failed!\n");
		return;
	}
}
