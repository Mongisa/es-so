#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

#define SIZE 100

typedef struct {
	char str[SIZE];
} shared_data;

void reverse_str(char *str) {
	int len = strlen(str);
	for(int i=0; i<len/2;i++) {
		char temp = str[i];
		str[i] = str[len - i - 1];
		str[len - i - 1] = temp;
	}
}

int main(int argc, char *argv[]) {
	char *string;
	
	if(argc != 2) {
		printf("Inserisci una stringa!\n");
		return 1;
	}

	string = argv[1];

	int segment_id = shmget(IPC_PRIVATE, sizeof(shared_data), S_IRUSR | S_IWUSR);

	if(segment_id<0) {
		printf("shmget error\n");
		return 1;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("Fork error!\n");
		shmctl(segment_id, IPC_RMID, NULL);
		return 1;
	}

	if(pid == 0) {
		//Child process
		shared_data *p = (shared_data *)shmat(segment_id, NULL, 0);
		reverse_str(string);
		strcpy(p->str,string);

		shmdt(p);
	} else {
		//Father process
		shared_data *p = (shared_data *)shmat(segment_id, NULL, 0);

		wait(NULL);

		printf("Reversed string: %s\n",p->str);

		shmdt(p);

		shmctl(segment_id, IPC_RMID, NULL);
	}
}
