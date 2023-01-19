#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>
#define MSGQ_ID 1337
#define MAX_LEN 255

typedef struct Message {
    long messageType;
    char messageContent[MAX_LEN];
} Message;

int main() {
    int qid = msgget(MSGQ_ID, IPC_CREAT | 0666);
    if (qid == -1) {
        fprintf(stderr, "Could not create a message queue!\n");
        exit(EXIT_FAILURE);
    }
    int pid = fork();
    if (pid > 0) {
        srand(time(0));
        Message buffer;
        int count = (rand() % 25) + 1;
        printf("Generating %d random numbers...\n", count);
        for (int i = 0; i < count; ++i) {
            int randomNumber = (rand() % 99) + 1;
            sprintf(buffer.messageContent, "%d", randomNumber);
            int sndStatus = msgsnd(qid, &buffer, sizeof(buffer.messageContent), 0);
            if (sndStatus == -1) fprintf(stderr, "Failed to sent number [%d]!\n", randomNumber);
            else printf("Sent [%d]\n", randomNumber);
        }
        strcpy(buffer.messageContent, "-1");
        int sndStatus = msgsnd(qid, &buffer, sizeof(buffer.messageContent), 0);
        if (sndStatus == -1) fprintf(stderr, "Failed to send termination character!\n");
        wait(NULL);
        msgctl(qid, IPC_RMID, 0);
    }
    else {
        Message buffer;
        do {
            int rcvStatus = msgrcv(qid, &buffer, sizeof(buffer.messageContent), 0, 0);
            if (rcvStatus == -1) fprintf(stderr, "Error: Could not receive the message!\n");
            else if (strcmp(buffer.messageContent, "-1") != 0) printf("Received: [%s]\n", buffer.messageContent);    
        } while (strcmp(buffer.messageContent, "-1") != 0);
    }
    return 0;
}