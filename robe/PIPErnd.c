#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 10

int main(int argi,int argv[]) {
    int fd[2], i, n, message[N];

    if (pipe(fd) < 0) {
        printf("Pipe creation error\n");
        return 1;
    }
    pid_t pid = fork();
    if (pid<0) {
        printf("fork error\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        close(fd[0]); // Close read pipe
        // Compose message
	srand(time(0));
	for(int i=0;i<N;i++) {
		message[i] = rand() % 100;
	}
        int sent = (int)write(fd[1],&message, sizeof(int)*N);
        if (sent != sizeof(int)*N) {
            printf("Sending error!\n");
            return 1;
        }
        close(fd[1]);
        return 0;
    } else {
        // Father process
        close(fd[1]); // Close write pipe
        int received = (int)read(fd[0], &message, sizeof(int)*N);
        if (received < 0) {
            printf("Receiving error!\n");
        } else {
            if (received < sizeof(int)*N)
                printf("Messagge incomplete!\n");
	    else {
	    	printf("Messaggio ricevuto:\n");
		for(int i=0;i<N;i++) {
			printf("%d\n",message[i]);
		}
	    }
        }
        close(fd[0]);
        return 0;
    }
}
