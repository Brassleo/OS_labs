#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void sigHandler(int signal)
{
    switch (signal) {
        case SIGINT:
            printf("\nSIGINT caught.\n");
            break;
        case SIGTERM:
            printf("\nSIGTERM caught.\n");
            break;
    }

    exit(0);
}

int main()
{
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);
    while(1);
}
