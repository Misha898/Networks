#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

const char message[] = "misha molodets\n";
int main(int argc, void *argv[]) {
    int simpleSocket = 0;
    int simplePort = 0;
    int returnStatus = 0;
    char buffer[256] = "";
    struct sockaddr_in simpleServer;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server> <port>\n", argv[0]);
        exit(1);
    }
    simpleSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (simpleSocket == -1) {
        fprintf(stderr, "Could not create a socket!\n");
        exit(1);
    }
    fprintf(stderr, "Socket created, %d\n", simpleSocket);
    simplePort = atoi(argv[2]);
    bzero(&simpleServer, sizeof(simpleServer));
    simpleServer.sin_family = AF_INET;
    simpleServer.sin_addr.s_addr = inet_addr(argv[1]);
    simpleServer.sin_port = htons(simplePort);
    returnStatus = connect(simpleSocket, (struct sockaddr *)&simpleServer, sizeof(simpleServer));
    if (returnStatus == 0) {
        fprintf(stderr, "Connect completed!\n");
    }
    else {
        fprintf(stderr, "Couldn't bind to address!\n");
        close(simpleSocket);
        exit(1);
    }
    returnStatus = read(simpleSocket, buffer, sizeof(buffer)); 
    if (returnStatus > 0) {
        printf("%d: %s", returnStatus, buffer);
    }
    else {
        fprintf(stderr, "Return Status = %d\n", returnStatus);
    }
    close(simpleSocket);
    return 0;
}

