#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
	pid_t pid1,pid2;

	pid1 = fork();
	pid2 = fork();

	//fork a child process
	if(pid1<0 || pid2<0) {
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	}

	if(pid1 >  0) {
		printf("[1]Father: PID=%d, getpid=%d\n",pid1,getpid());
		wait(NULL);
	} else if(pid1 == 0) {
		printf("[1]Child: PID=%d, getpid=%d\n",pid1,getpid());
	}

	if(pid2 > 0) {
		printf("[2]Father: PID=%d, getpid=%d\n",pid2,getpid());
		wait(NULL);
	} else if(pid2 == 0) {
		printf("[2]Child: PID=%d, getpid=%d\n",pid2,getpid());
	}
	return 0;
}
