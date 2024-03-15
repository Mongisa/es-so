#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    unsigned int tP = 2;
    unsigned int tC = 5;

    // fork a child process
    pid = fork();
    if(pid < 0) {
        // error occurred
        fprintf(stderr, "Fork failed\n");
    }
    else
        if (pid == 0) {
            // child process
            sleep(tC);
            fprintf(stdout, "Child: PID: %d, PID padre: %d, fork: %d\n", getpid(), getppid(), pid);
        } else {
            // parent process
            sleep(tP);
            fprintf(stdout, "Father: PID: %d, PID padre: %d, fork: %d\n", getpid(), getppid(), pid);
        }
    fprintf(stdout, "Program completed!\n");
}
