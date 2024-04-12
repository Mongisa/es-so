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
	int fd[2];
	
	if(argc != 2) {
		printf("Inserisci una stringa!\n");
		return 1;
	}

	string = argv[1];

	if(pipe(fd)<0) {
		printf("Pipe error!\n");
		return 1;
	}

	pid_t pid = fork();

	if(pid<0) {
		printf("Fork error!\n");
		return 1;
	}

	if(pid == 0) {
		//Child process
		close(fd[0]);

		shared_data p;

		reverse_str(string);
		strcpy(p.str,string);

		int sent = write(fd[1], &p, sizeof(shared_data));

		if(sent<sizeof(shared_data)) {
			printf("Sending error!\n");
			return 1;
		}
		close(fd[1]);
	} else {
		//Father process
		close(fd[1]);

		shared_data p;

		int received = read(fd[0], &p, sizeof(shared_data));

		if(received<sizeof(shared_data)) {
			printf("Receiving error!\n");
			return 1;
		}

		printf("Reversed string: %s\n",p.str);

		close(fd[0]);
	}
}
