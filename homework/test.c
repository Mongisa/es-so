#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]) {
	pid_t pid;

	pid = fork();

	if(pid<0) {
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	}

	if(pid>0) {
		//Father's code
		printf("CIAO\n");
		wait(NULL);
		printf("Comando completato!\n");
	} else if(pid == 0) {
		execlp("/bin/ls","ls","-l",NULL);
		printf("Error while executing %s command!\n",argv[1]);
	}
	return 0;
}
