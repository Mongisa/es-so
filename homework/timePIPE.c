#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	pid_t pid;
	int fd[2];
	struct timeval current;

	if(argc == 1) {
		printf("Nessun comando passato!\n");
		return 1;
	}

	if(pipe(fd)<0) {
		printf("Pipe error!\n");
		return 1;
	}

	pid = fork();

	if(pid<0) {
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}

	if(pid>0) {
		//Father's code
		close(fd[1]);

		long int time;

		gettimeofday(&current,NULL);
		wait(NULL);
		printf("Command completed!\n");
		int received = read(fd[0],&time,sizeof(long int));

		if(received < sizeof(long int)) {
			printf("Error occurred while receiving!\n");
			return 1;
		}

		long int delta = time - current.tv_usec;

		printf("Tempo esecuzione %s: %ld microsec\n",argv[1],delta);
		close(fd[0]);
	} else if(pid == 0) {
		//Child's code
		close(fd[0]);
 		gettimeofday(&current,NULL);
		int status = system(argv[1]);
		if(status < 0) {
			printf("Errore durante l'esecuzione del comando\n");
			return 1;
		}
		int sent = write(fd[1],&current.tv_usec,sizeof(long int));

		if(sent<sizeof(long int)) {
			printf("Error occurred while sending!\n");
			return 1;
		}
		close(fd[1]);
		return 0;
	}

	return 0;
}
