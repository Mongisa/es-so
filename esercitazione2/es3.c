#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define MAX_SIZE 100

void child_process(int,int);

int main() {
    int segment_id=shmget(IPC_PRIVATE, MAX_SIZE*sizeof(float), S_IRUSR | S_IWUSR);
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
        child_process(segment_id,1);
	pid_t pid_2 = fork();
	if(pid_2<0) {
		fprintf(stderr, "fork error\n");
		shmctl(segment_id, IPC_RMID, NULL);
		return 1;
	}
	if(pid_2 == 0) {
		child_process(segment_id,51);
		return 0;
	} else {
		wait(NULL);
	}

        return 0;
    } else {
        // Father process
        // Wait for child before accessing to shared memory
        wait(NULL);
        // Attach shared memory
        float *p = (float *)shmat(segment_id, NULL, 0);
        float sum = 0;
	for(int i=0;i<100;i++) {
		//printf("%f ",p[i]);
		sum += p[i];
	}
	//printf("\n");

	printf("Somma: %.3f\n",sum);
        // Detach shared memory
        shmdt(p);//Pointer
        // Important! Shared Memory deallocation
        shmctl(segment_id, IPC_RMID, NULL);
        return 0;
    }
}

void child_process(int segment_id,int i) {
	float *p=(float *)shmat(segment_id,NULL,0);

	for(i;i<=i+50;i++) {
		p[i-1]=(float)1/(float)i;
	}

	shmdt(p);
}
