#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXBUF 1024

int main(int argc, char *argv[]) {
    int udpSocket;
    int returnStatus = 0;
    int addrlen = 0;
    struct sockaddr_in udpServer, udpClient;
    char buf[MAXBUF];
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udpSocket == -1) {
        fprintf(stderr, "Couldn't create a socket\n");
        exit(1);
    }
    printf("Socket created\n");
    udpServer.sin_family = AF_INET;
    udpServer.sin_addr.s_addr = htonl(INADDR_ANY);
    udpServer.sin_port = htons(atoi(argv[1]));
    returnStatus = bind(udpSocket, (struct sockaddr*)&udpServer, sizeof(udpServer));
    if (returnStatus == 0) {
        fprintf(stderr, "Bind completed!\n");
    }
    else {
        fprintf(stderr, "Couldn't bind to address!\n");
        close(udpSocket);
        exit(1);
    }
    while (1) {
        addrlen = sizeof(udpClient);
        returnStatus = recvfrom(udpSocket, buf, MAXBUF, 0,
                (struct sockaddr *)&udpClient, &addrlen);
        if (returnStatus == -1) {
            fprintf(stderr, "Could not receive message!\n");
        }
        else {
            printf("Received: %s\n", buf);
            strcpy(buf, "OK");
            returnStatus = sendto(udpSocket, buf, strlen(buf) + 1, 0,
                    (struct sockaddr*)&udpClient, sizeof(udpClient));
            if (returnStatus == -1) {
                fprintf(stderr, "Could not send confirmation!\n");
            }
            else {
                printf("Confirmation sent\n");
            }
        }
    }
    close(udpSocket);
    return 0;
}
