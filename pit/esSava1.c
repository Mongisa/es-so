#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) {

	if(argc < 2) {
		printf("Fornisci almeno un comando!\n");
		return EXIT_FAILURE;
	}

	int segment_id = shmget(IPC_PRIVATE, sizeof(unsigned long long), S_IRUSR | S_IWUSR);

	if(segment_id<0) {
		printf("shmget error!\n");
		return EXIT_FAILURE;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("fork error\n");
		shmctl(segment_id, IPC_RMID, NULL);
		return EXIT_FAILURE;
	}

	if(pid > 0) {
		//Father process
		int* p = (int*)shmat(segment_id, NULL, 0);

		*p = 1 + (getppid() % (argc-1));

		printf("Sono il processo padre: %d\n",*p);

		wait(NULL);
		printf("Il figlio ha finito\n");
		shmdt(p);

		shmctl(segment_id, IPC_RMID, NULL);
	} else {
		//Child process
		int* p = (int*)shmat(segment_id, NULL, 0);

		printf("Sono il processo figlio: %d\n",*p);

		char path[30] = "/bin/";
		char cmd[30];

		strcpy(cmd,argv[*p]);
		strcat(path,cmd);

		shmdt(p);

		execlp(path,cmd,NULL);
	}

	return EXIT_SUCCESS;
}
