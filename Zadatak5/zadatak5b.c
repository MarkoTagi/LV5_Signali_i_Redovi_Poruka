#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MSGQID 1337
#define MAX_LEN 255

typedef struct Message {
    long messageType;
    char messageContent[MAX_LEN];
} Message;

int main() {
    int qid = msgget(MSGQID, IPC_CREAT | 0666);
    if (qid == -1) {
        fprintf(stderr, "Could not create a message queue :(\n");
        exit(EXIT_FAILURE);
    }
    Message buffer;
    do {
        int rcvStatus = msgrcv(qid, &buffer, sizeof(buffer.messageContent), 0, 0);
        if (rcvStatus == -1) fprintf(stderr, "Could not receive the number :(\n");
        else {
            if (strcmp(buffer.messageContent, "-1") != 0) printf("Received: [%s]\n", buffer.messageContent);
        }
    } while (strcmp(buffer.messageContent, "-1") != 0);
    return 0;
}