#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_val = 0;

int main() {
	pid_t pid;

	int* local_val = (int*)malloc(sizeof(int));

	//fork a child process
	pid = fork();
	if(pid<0) {
		fprintf(stderr, "Fork Failed\n");
		exit(1);
	}

	if(pid == 0) {
		global_val = 100;
		(*local_val)++;
		printf("Child: PID=%d global_val=%d local_val=%d\n",getpid(),global_val,*local_val);
		free(local_val);
	} else if(pid > 0) {
		printf("Father: PID=%d global_val=%d local_val=%d\n",getpid(),global_val,*local_val);
		wait(NULL);
	}

	return 0; 
}
