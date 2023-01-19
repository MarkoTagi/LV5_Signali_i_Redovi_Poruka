/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji onemogućava da se tekući
proces prekine jednim pritiskom kombinacije tastera Ctrl-C. Proces se prekida tek kada se ova
kombinacija tastera pritisne dva puta za redom.*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int intrCount = 0;

static void alrmHandler(int sigNo) {
    //printf("Timeout!\n");
    if (sigNo == SIGALRM) intrCount = 0;
} 

static void intrHandler(int sigNo) {
    if (sigNo == SIGINT) {
        signal(SIGALRM, &alrmHandler);
        ++intrCount;
        //printf("\n[Counter = %d]\n", intrCount);
        alarm(1);
        if (intrCount == 2) {
            alarm(0);
            signal(SIGINT, SIG_DFL);
            kill(getpid(), SIGINT);
        }
    }
}

int main() {
    if (signal(SIGINT, &intrHandler) == SIG_ERR) {
        fprintf(stderr, "Error: Signal could not be caught!\n");
        exit(EXIT_FAILURE);
    }
    while(1) pause();
    return 0;
}