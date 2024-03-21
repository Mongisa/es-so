#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define N 100

int main(int argc,char* argv[]) {
    int fd[2], i, n;
    //char message[N];

    if(argc == 1) {
    	printf("No arguments!\n");
	return 1;
    }

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
        //strcpy(message,argv[1]);
        int sent = (int)write(fd[1], argv[1], sizeof(argv[1]));
        if (sent != sizeof(argv[1])) {
            printf("Sending error!\n");
            return 1;
        }
        close(fd[1]);
        return 0;
    } else {
        // Father process
	// La sincronizzazione è gestita dal SO
        close(fd[1]); // Close write pipe
        int received = (int)read(fd[0], argv[1], sizeof(argv[1]));
        if (received < 0) {
            printf("Receiving error!\n");
        } else {
            if (received < sizeof(argv[1])) {
                printf("Messagge incomplete!\n");
		return 1;
	    }
	    printf("%s",argv[1]);
            printf("\n");
        }
        close(fd[0]);
        return 0;
    }
}
