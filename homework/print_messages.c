#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
	pid_t pid;

	for(int i=1;i<argc;i++) {

		pid = fork();

		if(pid<0) {
			fprintf(stderr, "Fork Failed\n");
			exit(1);
		}

		if(pid>0) {
			//Father's code
			wait(NULL);
		} else if(pid == 0) {
			srand(getpid());
			int n = (rand() % 11);
			sleep(n);
			printf("PID: %d, Message: %s, sleep_time: %ds\n",getpid(),argv[i],n);
			return 0;
		}
	}
	return 0;
}
