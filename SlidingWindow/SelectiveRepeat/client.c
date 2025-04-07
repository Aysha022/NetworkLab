#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>

// Simulate message corruption randomly
int is_corrupt() {
    return rand() % 3 == 0; // 25% chance to corrupt
}

int main() {
    srand(time(0)); // Seed random number generator

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server = {0};

    server.sin_family = AF_INET;
    server.sin_port = htons(9009);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    connect(sock, (struct sockaddr*)&server, sizeof(server));
    printf("Client started...\n");

    char buff[50];

    while (1) {
        bzero(buff, sizeof(buff));
        int n = read(sock, buff, sizeof(buff));
        if (n <= 0) break;

        printf("Received: %s\n", buff);
        char ack[20];
        int id = buff[strlen(buff) - 1] - '0';

        if (is_corrupt()) {
            sprintf(ack, "NACK-%d", id);
            printf("Simulating corruption... Sending: %s\n", ack);
        } else {
            sprintf(ack, "ACK-%d", id);
            printf("Message OK... Sending: %s\n", ack);
        }

        write(sock, ack, sizeof(ack));
    }

    printf("Client exiting...\n");
    close(sock);
    return 0;
}
