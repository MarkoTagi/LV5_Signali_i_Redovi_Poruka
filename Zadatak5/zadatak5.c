/*Korišćenjem programskog jezika C napisati UNIX/Linux program koji simulira problem
proizvođač/potrošač korišćenjem redova poruka (message-queues). Glavni program se deli u dva
procesa. Prvi proces (proizvođač) kreira N slučajnih pozitivnih celih brojeva i šalje ih drugom
procesu. N se određuje tokom izvršenja, takođe kao slučajan pozitivan ceo broj. Po završetku
slanja, prvi proces šalje -1 kao kod za kraj. Drugi proces (potrošač) preuzima poslate brojeve iz
poruka i štampa ih na standardnom izlazu.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#define MAX_LEN 255
#define MSGQ_ID 1337

typedef struct Message {
    long messageType;
    char messageContent[MAX_LEN];
} Message;

int main() {
    int qid = msgget(MSGQ_ID, IPC_CREAT | 0666);
    if (qid == -1) {
        fprintf(stderr, "Could not create a message queue [%d] :(\n", MSGQ_ID);
        exit(EXIT_FAILURE);
    }
    int pid = fork();
    if (pid == -1) {
        fprintf(stderr, "fork() has failed :(\n");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        srand(time(0));
        int count = (rand() % 20) + 1;
        int* randomNumbers = malloc(count * sizeof(int));
        printf("%d numbers to be sent...\n", count);
        Message buffer;
        for (int i = 0; i < count; ++i) {
            randomNumbers[i] = (rand() % 99) + 1;
            printf("#%d Randomly generated number [%d]\n", i, randomNumbers[i]);
            sprintf(buffer.messageContent, "%d", randomNumbers[i]);
            if (msgsnd(qid, &buffer, sizeof(buffer.messageContent), 0) == -1) fprintf(stderr, "Could not send the number :(\n");
            else printf("Succesfully sent the number :D\n");
        }
        strcpy(buffer.messageContent, "-1");
        if (msgsnd(qid, &buffer, sizeof(buffer.messageContent), 0) == -1) fprintf(stderr, "Could not send the number :(\n");
        else printf("Termination number sent...\n");
        wait(NULL);
        msgctl(qid, IPC_RMID, 0);
        free(randomNumbers); 
    }
    else {
       int execlStatus = execl("./zadatak5b", "./zadatak5b", NULL);
       if (execlStatus == -1) {
        fprintf(stderr, "execl() has failed :(\n");
        exit(EXIT_FAILURE);
       }
    }
    return 0;
}