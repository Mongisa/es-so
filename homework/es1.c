#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid;

	pid = fork();

	//fork a child process
	if(pid<0) {
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	}

	if(pid > 0) {
		printf("Father: PID=%d\n",getpid());
		wait(NULL);
	} else if(pid == 0) {
		printf("Child: PID=%d\n",getpid());
		pid = fork();
		if(pid > 0) {
			printf("Child now Father: PID=%d\n",getpid());
			wait(NULL);
		} else if (pid == 0) {
			printf("Child of the child: PID=%d\n",getpid());
		}
	}

	return 0; 
}
