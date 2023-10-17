#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sigHandler() {
}

int main() {
    pid_t child_pid;
    int status;

    child_pid = fork();

    if (child_pid == 0) {
        while(1);
        
    } else if (child_pid > 0) {
        signal(SIGINT, sigHandler);
        wait(&status);

        if (WIFEXITED(status)) 
            printf("\nChild process exited with code: %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
	    printf("\nChild process stopped by signal %d\n", WTERMSIG(status));

    } else {
        perror("fork");
        exit(1);
    }

    return 0;
}
