#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	pid_t pid;
	struct timeval current;

	int segment_id=smget(IPC_PRIVATE,sizeof(current),S_IRUSR|S_IWUSR);

	pid = fork();

	if(pid<0) {
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	}

	if(pid>0) {
		//Father's code
		wait(NULL);
		printf("Comando completato!\n");
	} else if(pid == 0) {
		char path[30] = "/bin/";
		strcat(path,argv[1]);
		execlp(path,argv[1],NULL);
		printf("Error while executing %s command!\n",argv[1]);
	}
	return 0;
}
