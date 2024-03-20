#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define MAX_SIZE 200

int main() {
    int segment_id=shmget(IPC_PRIVATE, MAX_SIZE, S_IRUSR | S_IWUSR);
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
        char *p=(char *)shmat(segment_id, NULL, 0); //Returns void* pointer --> casting to char*
        strcpy(p, "Hello World!");
        // Detach shared memory
        shmdt(p);
        return 0;
    } else {
        // Father process
        // Wait for child before accessing to shared memory
        wait(NULL);
        // Attach shared memory
        char *string = (char *)shmat(segment_id, NULL, 0);
        printf("Message received: %s\n", string);
        // Detach shared memory
        shmdt(string);//Pointer
        // Important! Shared Memory deallocation
        shmctl(segment_id, IPC_RMID, NULL);
        return 0;
    }
}
