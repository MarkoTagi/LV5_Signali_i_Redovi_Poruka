/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji učitava podatke iz tekstualne
datoteke cela (red po red) i zatim korišćenjem reda poruka sve parne redove šalje procesu koji
konvertuje sva slova u velika i zapisuje ih u datoteku pola1, a sve neparne redove procesu koji
konvertuje sva slova u mala i zapisuje ih u datoteku pola2.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#define MSGQID 1337
#define MAX_LEN 255

typedef struct Message {
    long messageType;
    char messageContent[MAX_LEN];
} Message;

int main() {
    char fileName[] = {"cela.txt"};
    FILE* filep = fopen(fileName, "r");
    if (filep == NULL) {
        fprintf(stderr, "Could not open file \"%s\"!\n", fileName);
        exit(EXIT_FAILURE);
    }
    int msgqid = msgget(MSGQID, IPC_CREAT | 0666); //0666: Read and write for the owner, the group and others
    if (msgqid == -1) {
        fprintf(stderr, "Could not create the message queue!\n");
        exit(EXIT_FAILURE);
    }    
    int pid = fork();
    if (pid == -1) {
        fprintf(stderr, "[1] fork() has failed!\n");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) {
        int execlStatus = execl("./zadatak6b", "./zadatak6b", "1", NULL);
        if (execlStatus == -1) {
            fprintf(stderr, "[1] execl() has failed!\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        pid = fork();
        if (pid == -1) {
            fprintf(stderr, "[2] fork() has failed!\n");
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            int execlStatus = execl("./zadatak6b", "./zadatak6b", "2", NULL);
            if (execlStatus == -1) {
                fprintf(stderr, "[2] execl has failed!\n");
                exit(EXIT_FAILURE);
            }
        }
        else {
            Message buffer;
            char line[MAX_LEN];
            int lineCount = 0;
            while (fgets(line, MAX_LEN, filep) != NULL) {
                int messageLength = strlen(line);
                line[--messageLength] = '\0';
                if ((lineCount % 2) == 0) {
                    strcpy(buffer.messageContent, line);
                    buffer.messageType = 1;
                    //printf("Buffer --> messageType: %ld.\nBuffer --> messageContent: %s\n", buffer.messageType, buffer.messageContent);
                    if (msgsnd(msgqid, &buffer, MAX_LEN, 0) == -1) fprintf(stderr, "Error! [msgsnd]\n");
                    //else printf("Sending line...\n");
                }
                else {
                    strcpy(buffer.messageContent, line);
                    buffer.messageType = 2;
                    //printf("Buffer --> messageType: %ld.\nBuffer --> messageContent: %s.\n", buffer.messageType, buffer.messageContent);
                    if (msgsnd(msgqid, &buffer, MAX_LEN, 0) == -1) fprintf(stderr, "Error! [msgsnd]\n");
                    //else printf("Sending line...\n", sendStatus);
                }
                ++lineCount;
            }
            fclose(filep);
            strcpy(buffer.messageContent, "END");
            buffer.messageType = 1;
            if (msgsnd(msgqid, &buffer, MAX_LEN, 0) == -1) fprintf(stderr, "Error! [msgsnd]\n");
            //else printf("Sending termination line...\n");
            buffer.messageType = 2;
            if (msgsnd(msgqid, &buffer, MAX_LEN, 0) == -1) fprintf(stderr, "Error! [msgsnd]\n");
            //else printf("Sending termination line...\n");
            wait(NULL);
            wait(NULL);
            msgctl(msgqid, IPC_RMID, 0);
        }
    }
    return 0;
}