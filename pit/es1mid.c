#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define SIZE 100

int main(int argc, char* argv[]) {
	typedef char stringa[100];
	
	int segment_id = shmget(IPC_PRIVATE, 3*sizeof(stringa), S_IRUSR | S_IWUSR);

	if(segment_id<0) {
		printf("shmget error\n");
		return 1;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("fork error\n");
		shmctl(segment_id, IPC_RMID, NULL);
		return 1;
	}

	if(pid == 0) {
		//Child process
		stringa* data = (stringa*)shmat(segment_id, NULL, 0);
		
		for(int i=1;i<=3;i++) {
			printf("Inserisci %da stringa: ",i);
			scanf("%99s",data[i-1]);
		}

		shmdt(data);
	} else {
		//Father process
		stringa* data = (stringa*)shmat(segment_id, NULL, 0);
		wait(NULL);

		for(int i=1;i<=3;i++) {
			printf("Stringa %d: %s\n",i,data[i-1]);
		}

		shmctl(segment_id, IPC_RMID, NULL);

		shmdt(data);
	}
	return 0;
}
