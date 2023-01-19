/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji može da se prekine samo
ako se kombinacija tastera Ctrl-C pritisne pet puta za redom. Ukoliko korisnik pritisne kombinaciju
tastera Ctrl-Z program na ekranu ispisuje koliko puta do sada je pritisnuto Ctrl-C.*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int intCount = 0;

static void alrmHandler(int sigNo) {
    printf("Timeout!\n");
    intCount = 0;
}

static void intHandler(int sigNo) {
    signal(SIGALRM, &alrmHandler);
    ++intCount;
    alarm(1);
    if (intCount == 5) {
        signal(SIGINT, SIG_DFL);
        kill(getpid(), SIGINT);
    }
}

static void tstpHandler(int sigNo) {
    printf("\n[Counter = %d]\n", intCount);
}

int main() {
    signal(SIGINT, &intHandler);
    signal(SIGTSTP, &tstpHandler);
    while(1) pause();
    return 0;
}