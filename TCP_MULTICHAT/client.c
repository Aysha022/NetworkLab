#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int sock;

void* receive_messages(void* arg) {
    char buffer[BUFFER_SIZE];

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            close(sock);
            exit(1);
        }

        printf("\nMessage: %s\n> ", buffer);
        fflush(stdout);
    }

    return NULL;
}

int main() {
    struct sockaddr_in server_addr;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sock == -1) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to chat server.\n");

    pthread_t tid;
    pthread_create(&tid, NULL, receive_messages, NULL);
    pthread_detach(tid);

    while (1) {
        char message[BUFFER_SIZE];
        printf("> ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = 0; // Remove newline

        send(sock, message, strlen(message), 0);
    }

    close(sock);
    return 0;
}
