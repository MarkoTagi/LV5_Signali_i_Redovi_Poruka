#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#define MSGQID 1337
#define MAX_LEN 255

typedef struct Message  {
    long messageType;
    char messageContent[MAX_LEN];
} Message;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "bad usage: zadatak6b\n");
        exit(EXIT_FAILURE);
    }
    int msgqid = msgget(MSGQID, IPC_CREAT | 0666);
    if (msgqid == -1) {
        fprintf(stderr, "Could not create the message queue!\n");
        exit(EXIT_FAILURE);
    }
    Message buffer;
    if (strcmp(argv[1], "1") == 0) { //Konvertuje sva slova u velika
        char fileName[] = {"pola1.txt"};
        FILE* filep = fopen(fileName, "w");
        if (filep == NULL) {
            fprintf(stderr, "Could not open file \"%s\"!\n", fileName);
            exit(EXIT_FAILURE);
        }
        do {
            int rcvStatus = msgrcv(msgqid, &buffer, MAX_LEN, 1, 0);
            if (rcvStatus == -1) fprintf(stderr, "Something went wrong: Could not receive the message :(\n");
            else {
                printf("Message received! [%s]\n", buffer.messageContent);
                if (strcmp(buffer.messageContent, "END") != 0) {
                    int messageLength = strlen(buffer.messageContent);
                    for (int i = 0; i < messageLength; ++i) buffer.messageContent[i] -= (buffer.messageContent[i] >= 'a' && buffer.messageContent[i] <= 'z') ? 32 : 0;
                    fputs(buffer.messageContent, filep);
                }
            }
        } while (strcmp(buffer.messageContent, "END") != 0);
        fclose(filep);
    }
    else { //Konvertuje sva slova u mala
        char fileName[] = {"pola2.txt"};
        FILE* filep = fopen(fileName, "w");
        if (filep == NULL) {
            fprintf(stderr, "Could not open file \"%s\"!\n", fileName);
            exit(EXIT_FAILURE);
        }
        do {
            int rcvStatus = msgrcv(msgqid, &buffer, MAX_LEN, 2, 0);
            if (rcvStatus == -1) fprintf(stderr, "Something went wrong: Could not receive the message :(\n");
            else {
                printf("Message received! [%s]\n", buffer.messageContent);
                if (strcmp(buffer.messageContent, "END") != 0) {
                    int messageLength = strlen(buffer.messageContent);
                    for (int i = 0; i < messageLength; ++i) buffer.messageContent[i] += (buffer.messageContent[i] >= 'A' && buffer.messageContent[i] <= 'Z') ? 32 : 0;
                    fputs(buffer.messageContent, filep);
                }
            }
        } while (strcmp(buffer.messageContent, "END") != 0);
        fclose(filep);
    }
    return 0;
}