#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 5

void message_read(int*,int*,int);

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
        close(fd[0]); // Close read pipe
	for(int i=0;i<N;i++) {
		printf("Inserisci numero [%d]: ",i+1);	
		scanf(" %d",&num[i]);
	}
	int sent = (int)write(fd[1],num,2*sizeof(int));
	if(sent != 2*sizeof(int)) {
		printf("Sending error\n");
		return 1;
	}

	sleep(1);

	sent = (int) write(fd[1],num+2,3*sizeof(int));
	if(sent != 3*sizeof(int)) {
		printf("Sending error 2\n");
		return 1;
	}

        close(fd[1]);
        return 0;
    } else {
        // Father process
        close(fd[1]); // Close write pipe
       	
	message_read(num,fd,N);
	for(int i=0;i<N;i++) {
		printf("Ricevuto [%d]: %d\n",i+1,num[i]);
	}

        close(fd[0]);
        return 0;
    }
}

void message_read(int p[], int fd[], int el) {
	
	int count = 0;

	do {
		int received = (int)read(fd[0],p+count/sizeof(int),el*sizeof(int));
	
		if(received < 0 ) {
			printf("Receiving error!\n");
		} else if(received < sizeof(int)) {
			printf("Received partial message\n");
		} else {
			count += received;
		}


	} while(count != el*sizeof(int));
}
