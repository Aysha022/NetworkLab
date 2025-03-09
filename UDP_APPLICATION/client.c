#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define SAI struct sockaddr_in

void chat(int sockfd, SAI server) {
    printf("\nClient ready....\n");
    char buff[MAX];
    int num,len = sizeof(server);

    while (1) {      
        bzero(buff, sizeof(buff)); 
        printf("\nEnter a number (or type 'exit' to quit): ");
        scanf("%s", buff);

        // Exit condition
        if (strncmp(buff, "exit", 4) == 0) {
            printf("Client Exit...\n");
            break;
        }

        // Send number to server
        sendto(sockfd, buff, sizeof(buff), 0, (SA*)&server,len);

        // Receive response from server
        bzero(buff, sizeof(buff)); 
        recvfrom(sockfd, buff, sizeof(buff), 0, (SA*)&server,(socklen_t*)&len);

        // Display server response
        printf("\nServer: %s\n", buff);
    }
}

int main() {
    SAI server;
    int sockfd;
    
    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Socket creation failed...\n");
        exit(0);
    }
    printf("Socket successfully created..\n");

    // Initialize server details
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Start communication
    chat(sockfd, server);

    close(sockfd);
    return 0;
}
