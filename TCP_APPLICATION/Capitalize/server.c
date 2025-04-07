#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctype.h> // For isspace() and toupper()

#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define SAI struct sockaddr_in

// Function to capitalize the first letter of each word
void capitalizeFirstLetters(char *str) {
    int capitalize = 1; // Flag to indicate if the next character should be capitalized
    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace(str[i])) {
            capitalize = 1;
        } else if (capitalize && isalpha(str[i])) {
            str[i] = toupper(str[i]);
            capitalize = 0;
        }
    }
}

void chat(int connfd) {
    char buff[MAX];
    while (1) {
        // Clear the buffer
        bzero(buff, MAX);

        // Read message from the client
        read(connfd, buff, sizeof(buff));
        printf("Client: %s", buff);

        // Exit condition
        if (strncmp("exit", buff, 4) == 0) {
            printf("Client has exited the chat...\n");
            break;
        }

        // Capitalize the first letter of each word
        capitalizeFirstLetters(buff);

        // Send the processed message back to the client
        write(connfd, buff, sizeof(buff));
    }
}

int main() {
    SAI server, client;

    // Socket creation
    int sockfd, connfd, len;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created...\n");
    }

    // Initialize server details
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (SA*)&server, sizeof(server)) != 0) {
        printf("Socket bind failed...\n");
        exit(0);
    } else {
        printf("Socket successfully binded...\n");
    }

    // Listen for connections
    if (listen(sockfd, 5) != 0) {
        printf("Listen failed...\n");
        exit(0);
    } else {
        printf("Server listening...\n");
    }

    // Accept the client connection
    len = sizeof(client);
    connfd = accept(sockfd, (SA*)&client, (socklen_t*)&len);
    if (connfd < 0) {
        printf("Server accept failed...\n");
        exit(0);
    } else {
        printf("Server accepted the client...\n");
    }

    // Start chat
    chat(connfd);

    // Close the socket
    close(sockfd);
    return 0;
}
