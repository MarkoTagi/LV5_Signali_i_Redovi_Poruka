/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji svom procesu detetu,
korišćenjem redova poruka, prosleđuje ulaz koji prima preko tastature, a process dete dobijene
poruke štampa na ekranu. Predvideti da se unosom teksta “QUIT” prekida rad programa.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#define MSGQID 1337
#define MAX_LEN 255

typedef struct Message {
    long messageType;
    char messageContent[MAX_LEN];
} Message;

int main() {
    int qid = msgget(MSGQID, IPC_CREAT | 0666);
    if (qid == -1) {
        fprintf(stderr, "Could not create a message queue!\n");
        exit(EXIT_FAILURE);
    }
    int pid = fork();
    if (pid == -1) {
        fprintf(stderr, "fork() has failed!\n");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        Message buffer;
        do {
            printf("Enter something: ");
            fgets(buffer.messageContent, MAX_LEN, stdin);
            int inputLength = strlen(buffer.messageContent);
            buffer.messageContent[--inputLength] = '\0';
            if (msgsnd(qid, &buffer, sizeof(buffer.messageContent), 0) == -1) fprintf(stderr, "Could not send the message :(\n");
            else printf("Sending...\n");
        } while (strcmp(buffer.messageContent, "QUIT") != 0);
        wait(NULL);
        msgctl(qid, IPC_RMID, 0);
    }
    else {
        if (execl("./zadatak4b", "zadatak4b", NULL) == -1) {
            fprintf(stderr, "execl() has failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}