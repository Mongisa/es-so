#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 5

int main() {
    int fd[2], n, num[N];

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
	int a;
        close(fd[0]); // Close read pipe
	for(int i=0;i<N;i++) {
		printf("Inserisci numero [%d]: ",i+1);
		fflush(stdout);	
		scanf(" %d",&a);
	}
        // Compose message
        close(fd[1]);
        return 0;
    } else {
        // Father process
        close(fd[1]); // Close write pipe
	/*for(int i=0;i<50;i++) {
        	int received = (int)read(fd[0], &dato, sizeof(dato));
        	if (received < 0) {
            		printf("Receiving error!\n");
        	} else {
            		if (received < sizeof(dato))
               			printf("Messagge incomplete!\n");
            		// verify bytes received
            		printf("x:%.2f y:%.2f z:%.2f\n",dato.x,dato.y,dato.z);
        	}
	}*/
        close(fd[0]);
        return 0;
    }
}
