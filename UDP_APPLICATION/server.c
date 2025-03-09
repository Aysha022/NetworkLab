#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX 1024
#define PORT 8080
#define SA struct sockaddr
#define SAI struct sockaddr_in

// Function to check if a number is prime
int isPrime(int num) {
    if (num <= 1) return 0;
    for (int i = 2; i <= num; i++) {
        if (num % i == 0) return 0;
    }
    return 1;
}

// Function to get factors of a number
void getFactors(int num, char *result) {
    char temp[MAX];
    strcpy(result, "Composite. Factors: ");
    
    for (int i = 1; i <= num; i++) {
        if (num % i == 0) {
            sprintf(temp, "%d ", i);
            strcat(result, temp);
        }
    }
}

void chat(int sockfd, SAI client) {
    printf("\nServer ready, waiting for client...\n");
    char buff[MAX];
    int num,len = sizeof(client);
    
    while (1) {      
        bzero(buff, sizeof(buff));

        // Receive number from client
        recvfrom(sockfd, buff, sizeof(buff), 0, (SA*)&client, (socklen_t*)&len);
        printf("\nClient sent: %s\n", buff);
        
        // Convert received string to integer
        num = atoi(buff);
        
        // Prepare response
        bzero(buff, sizeof(buff));
        if (isPrime(num)) {
            sprintf(buff, "The number %d is Prime", num);
        } else {
            getFactors(num, buff);
        }

        // Send response to client
        sendto(sockfd, buff, sizeof(buff), 0, (SA*)&client,len);
    }
}

int main() {
    SAI server, client;
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

    // Bind socket
    if (bind(sockfd, (SA*)&server, sizeof(server)) == 0) 
        printf("Socket successfully binded..\n");

    // Start communication
    chat(sockfd, client);

    close(sockfd);
    return 0;
}
