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
		execlp("/bin/ls","ls","-al", NULL);
		//the next line is executed only if there is an error in execlp()
		printf("I'm the line after executing directory listing: ls -al \n");
	}

	//return 0; 
}
