#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/wait.h>

void fibonacci(int vec[], int size) {
    if (size <= 0) return;

    vec[0] = 0;
    if (size == 1) return;

    vec[1] = 1;
    if (size == 2) return;

    // Riempimento del vettore con la successione di Fibonacci
    for (int i = 2; i < size; i++) {
        vec[i] = vec[i - 1] + vec[i - 2];
    }
}

int main(int argc, char *argv[]) {
	if(argc != 2) {
		printf("Numero non fornito\n");
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);

	int segment_id = shmget(IPC_PRIVATE,n*sizeof(int),S_IRUSR | S_IWUSR);

	if(segment_id<0) {
		printf("shmget error\n");
		return EXIT_FAILURE;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("fork error\n");
		shmctl(segment_id, IPC_RMID, 0);
		return EXIT_FAILURE;
	}

	if(pid == 0) {
		//Child process
		int* data = shmat(segment_id, NULL, 0);

		fibonacci(data,n);

		shmdt(data);
	} else {
		//Father process
		int* data = shmat(segment_id, NULL, 0);
		wait(NULL);

		printf("Fibonacci: ");

		for(int i=0;i<n;i++) {
			printf("%d ",data[i]);
		}

		printf("\n");

		shmdt(data);
		shmctl(segment_id, IPC_RMID, 0);
	}
	return EXIT_SUCCESS;
}
