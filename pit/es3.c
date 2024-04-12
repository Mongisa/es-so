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

int is_palindrome(char *str1, char *str2) {
	return (strcmp(str1,str2) == 0) ? 1 : 0;
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
		strcpy(p->str,string);
		reverse_str(p->str);

		pid_t pid2 = fork();
		if(pid2<0) {
			printf("Fork error!\n");
			shmctl(segment_id, IPC_RMID, NULL);
			return 1;
		}

		if(pid2 == 0) {
			//Child child code
			if(is_palindrome(string,p->str) == 1) {
				printf("La stringa è palindroma!\n");
			} else {
				printf("La stringa non è palindroma!\n");
			}
		} else {
			//Child code
			wait(NULL);
		}

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
