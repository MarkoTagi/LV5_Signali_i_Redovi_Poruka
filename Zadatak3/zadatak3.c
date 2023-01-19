/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji od korisnika očekuje da sa
tastature unese korisničko ime. Ukoliko korisnik ne unese ništa u roku od 30 sekundi proces
obaveštava korisnika da je vreme za unos isteklo i izlazi.*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#define MAX_LEN 255
#define TIMEOUT 30
static void alrmHandler(int sigNo) {
    printf("\nTimeout! [%ds]\n", TIMEOUT);
    exit(EXIT_FAILURE);
}

int main() {
    signal(SIGALRM, &alrmHandler);
    char username[MAX_LEN];
    printf("Username: ");
    alarm(TIMEOUT);
    fgets(username, MAX_LEN, stdin);
    alarm(0);
    int nameLength = strlen(username);
    username[--nameLength] = '\0';
    printf("Username entered [%s]\n", username);
    return 0;
}