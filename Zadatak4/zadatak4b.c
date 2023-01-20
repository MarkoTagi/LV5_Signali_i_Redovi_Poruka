#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MSGQID 1337
#define MAX_LEN 255

typedef struct Message {
    long messageType;
    char messageConent[MAX_LEN];
} Message;

int main() {
    int qid = msgget(MSGQID, IPC_CREAT | 0666);
    if (qid == -1) {
        fprintf(stderr, "Could not create a message queue!\n");
        exit(EXIT_FAILURE);
    }
    Message buffer;
    do {
        int rcvStatus = msgrcv(qid, &buffer, sizeof(buffer.messageConent), 0, 0);
        if (rcvStatus == -1) fprintf(stderr, "Could not receive the message :(");
        else if (strcmp(buffer.messageConent, "QUIT") != 0) {
            printf("Received: [%s]\n", buffer.messageConent); 
            fflush(stdout);
        }
        buffer.messageType = getppid();
        msgsnd(qid, &buffer, sizeof(buffer.messageConent), 0);
    } while (strcmp(buffer.messageConent, "QUIT") != 0);
    return 0;
}