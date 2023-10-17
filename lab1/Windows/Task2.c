#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

void *randomNumberGenerator() {

    while (1) {
        double random = (double)rand() / RAND_MAX;
        printf("%.2f\n", random);
        sleep(3);
    }

}

void *keyboardInputHandler() {
    char input[256];

    while (1) {
        if (fgets(input, sizeof(input), stdin) != NULL) {
            int len = strlen(input);
            for (int i = len - 2; i >= 0; i--) {
                putchar(input[i]);
            }
            putchar('\n');
        }
    }
}

void sigintHandler() {
    printf("Received SIGINT. Exiting...\n");
    exit(1);
}

int main() {
    pthread_t randomNumberThread, keyboardInputThread;

    signal(SIGINT, sigintHandler);

    if (pthread_create(&randomNumberThread, NULL, randomNumberGenerator, NULL) != 0) {
        perror("Failed to create the random number thread");
        return 1;
    }

    if (pthread_create(&keyboardInputThread, NULL, keyboardInputHandler, NULL) != 0) {
        perror("Failed to create the keyboard input thread");
        return 1;
    }

    pthread_join(randomNumberThread, NULL);
    pthread_join(keyboardInputThread, NULL);

    return 0;
}
