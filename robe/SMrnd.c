#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define N 10

int main() {
    int segment_id=shmget(IPC_PRIVATE, N, S_IRUSR | S_IWUSR);
    if (segment_id<0) { //Segment identifier
        printf("shmget error\n");
        return 1;
    }

    pid_t pid=fork();
    if (pid<0) {
        fprintf(stderr, "fork error\n");
        // Important! Shared Memory deallocation
        shmctl(segment_id, IPC_RMID, NULL);
        return 1;
    } else if (pid==0) {
        // Child process
        // Attach shared memory
        int *p=(int *)shmat(segment_id, NULL, 0); //Returns void* pointer --> casting to char*
	srand(2);
	for(int i=0;i<N;i++) {
		p[i] = rand() % 100;
	}

        // Detach shared memory
        shmdt(p);
        return 0;
    } else {
        // Father process
        // Wait for child before accessing to shared memory
        wait(NULL);
        // Attach shared memory
        int *p = (int *)shmat(segment_id, NULL, 0);

	for(int i=0;i<N;i++) {
		printf("Numero %d: %d\n",i+1,p[i]);
	}
        // Detach shared memory
        shmdt(p);//Pointer
        // Important! Shared Memory deallocation
        shmctl(segment_id, IPC_RMID, NULL);
        return 0;
    }
}
