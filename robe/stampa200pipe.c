#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int fd[2];
    char *message;
    message=(char *)malloc(sizeof(char)*200);
    pid_t pid;
    if(pipe(fd)<0){
    	printf("pipe failed\n");
    	return 1;
    }
    pid=fork();
    if(pid<0){
    	printf("Fork failed\n");
    
    }
    if(pid==0){
   
    	close(fd[0]);
    	
    	printf("Inserisci il messaggio:\n");
    	scanf("%199s",message);
    	int sent=(int)write(fd[1],message,sizeof(char)*(strlen(message)+1));
    	
    	if(sent!=sizeof(char)*(strlen(message)+1)){
    		printf("Error\n");
    		return 1;
    	}
    	close(fd[1]);
    	return 0;
    }else{
    	close(fd[1]);
    	int received=(int)read(fd[0],message,sizeof(char)*200);
    	if(received<0){
    		printf("errore!");
    		return 1;
    	}else{
    	if(received<sizeof(char)*(strlen(message)+1)){
    		printf("Messaggio parziale\n");
    	}
    		printf("Messaggio ricevuto: %s\n",message);
    	}
    	close(fd[0]);
    	return 0;
    }
    free(message);
}
