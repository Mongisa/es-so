#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>

void ordina (float array[], int n) {
	int i, j, jmin;
	float temp;
	for(i=0;i<n-1;i++) {
		jmin=i;
		for(j=i+1;j<n;j++)
			if(array[j]<array[jmin])
				jmin=j;
		temp=array[i];
		array[i]=array[jmin];
		array[jmin]=temp;
	}
}

int main(int argc, char *argv[]) {

	if(argc != 2) {
		printf("Inserisci un numero!\n");
		return EXIT_FAILURE;
	}

	int n = atoi(argv[1]);

	int segment_id = shmget(IPC_PRIVATE, n*sizeof(float), S_IRUSR | S_IWUSR);

	if(segment_id<0) {
		printf("shmget error\n");
		return EXIT_FAILURE;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("fork error\n");
		shmctl(segment_id, IPC_RMID, NULL);
		return EXIT_FAILURE;
	}

	if(pid == 0) {
		//Child process
		float *data = (float*)shmat(segment_id, NULL, 0);

		for(int i=0;i<n;i++) {
			printf("Inserisci %d numero: ",i+1);
			scanf("%f",&data[i]);
		}

		ordina(data,n);

		shmdt(data);
	} else {
		//Father process
		float *data = (float*)shmat(segment_id, NULL, 0);
		wait(NULL);

		printf("Array ordinato: ");

		for(int i=0;i<n;i++) {
			printf("%.2f ",data[i]);
		}

		printf("\n");

		shmdt(data);
	}
}
