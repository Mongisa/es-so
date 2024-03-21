#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 10

struct cord {
	float x;
	float y;
	float z;
};

int main() {
    int fd[2], n;

    if (pipe(fd) < 0) {
        printf("Pipe creation error\n");
        return 1;
    }
    pid_t pid = fork();
    if (pid<0) {
        printf("fork error\n");
        return 1;
    } else if (pid != 0) {
        // Father process
        close(fd[0]); // Close read pipe
        // Compose message
	struct cord dato;
        for(int i=0; i<50; i++) {
            	dato.x = i;
		dato.y = i*2;
		dato.z = 3.0*i/2.0;
        	int sent = (int)write(fd[1], &dato, sizeof(dato));
        	if (sent != sizeof(dato)) {
            		printf("Sending error!\n");
            		return 1;
        	}
	}
        close(fd[1]);
        return 0;
    } else {
        // Child process
	// La sincronizzazione è gestita dal SO
	struct cord dato;
        close(fd[1]); // Close write pipe
	for(int i=0;i<50;i++) {
        	int received = (int)read(fd[0], &dato, sizeof(dato));
        	if (received < 0) {
            		printf("Receiving error!\n");
        	} else {
            		if (received < sizeof(dato))
               			printf("Messagge incomplete!\n");
            		// verify bytes received
            		printf("x:%.2f y:%.2f z:%.2f\n",dato.x,dato.y,dato.z);
        	}
	}
        close(fd[0]);
        return 0;
    }
}
