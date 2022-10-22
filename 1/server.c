#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

const char message[] = "misha molodets\n";
int main(int argc, void *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    struct sockaddr_in simpleServer;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (simpleSocket == -1) {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }
    fprintf(stderr, "Socket created, %d\n", simpleSocket);
    simplePort = atoi(argv[1]);
    bzero(&simpleServer, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = htonl(INADDR_ANY);
    simpleServer.sin_port = htons(simplePort);
    int returnStatus = bind(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));
    if (returnStatus == 0) {
        fprintf(stderr, "Bind completed!\n");
    }
    else {
        fprintf(stderr, "Couldn't bind to address!\n");
        close(simpleSocket);
        exit(1);
    }
    returnStatus = listen(simpleSocket, 5);
    if (returnStatus == -1) {
        fprintf(stderr, "Cannot listen on socket!\n");
        close(simpleSocket);
        exit(1);
    }
    while (1) {
        struct sockaddr_in clientName = {0};
        int simpleChildSocket = 0;
        int clientNameLength = sizeof(clientName);
        simpleChildSocket = accept(simpleSocket, (struct sockaddr *)&clientName, &clientNameLength);
        if (simpleChildSocket == -1) {
            fprintf(stderr, "Cannot accept connections!\n");
            close(simpleSocket);
            exit(1);
        }
        write(simpleChildSocket, message, strlen(message));
        close(simpleChildSocket);
    }
    close(simpleSocket);
    return 0;
}

