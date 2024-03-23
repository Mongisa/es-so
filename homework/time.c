#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	pid_t pid;
	struct timeval current;

	if(argc == 1) {
		printf("Nessun comando passato!\n");
		return 1;
	}

	int segment_id=shmget(IPC_PRIVATE,sizeof(current),S_IRUSR | S_IWUSR);
	if(segment_id<0) {
		printf("Errore in shmget\n");
		return 1;
	}

	pid = fork();

	if(pid<0) {
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}

	if(pid>0) {
		//Father's code
		gettimeofday(&current,NULL);
		wait(NULL);
		printf("Comando completato!\n");
		long int *array = shmat(segment_id,NULL,0);

		long int delta = array[0] - current.tv_usec;

		printf("Tempo esecuzione %s: %ld microsec\n",argv[1],delta);
		shmdt(array);
	} else if(pid == 0) {
		//Child's code
		long int *array = shmat(segment_id,NULL,0); 
		int status = system(argv[1]);
		if(status < 0) {
			printf("Errore durante l'esecuzione del comando\n");
			return 1;
		}
		gettimeofday(&current,NULL);
		array[0]=current.tv_usec;
		shmdt(array);
		return 0;
	}

	/*Deallocazione area di memoria*/
	shmctl(segment_id, IPC_RMID, NULL);

	return 0;
}
