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

	*string = argv[1];

	pid_t pid = fork();

	if(pid<0) {
		
	}
}
