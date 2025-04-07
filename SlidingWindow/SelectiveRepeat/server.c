#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void resend_message(int id, int sock) {
    char msg[50];
    sprintf(msg, "server-msg-%d", id);
    printf("Resending: %s\n", msg);
    write(sock, msg, sizeof(msg));
}

int main() {
    int s_sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server = {0}, client;

    server.sin_family = AF_INET;
    server.sin_port = htons(9009);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s_sock, (struct sockaddr*)&server, sizeof(server));
    listen(s_sock, 5);

    socklen_t len = sizeof(client);
    int c_sock = accept(s_sock, (struct sockaddr*)&client, &len);

    printf("Server started...\n");

    int base = 0;
    int window_size = 3;
    char buff[50];
    int total_messages = 9;

    while (base < total_messages) {
        printf("\nSending Window (base = %d)\n", base);

        // Send window of messages
        for (int i = base; i < base + window_size && i < total_messages; i++) {
            sprintf(buff, "server-msg-%d", i);
            write(c_sock, buff, sizeof(buff));
            printf("ent: %s\n", buff);
        }

        // Read ACKs or NACKs
        for (int i = base; i < base + window_size && i < total_messages; i++) {
            bzero(buff, sizeof(buff));
            read(c_sock, buff, sizeof(buff));

            int ack_id = buff[strlen(buff) - 1] - '0';

            if (buff[0] == 'N') {
                printf("NACK received for msg-%d\n", ack_id);
                resend_message(ack_id, c_sock);
                i--; // Retry this same message
            } else {
                printf("ACK received for msg-%d\n", ack_id);
                base++;
            }
        }
    }

    printf("\nAll messages sent and acknowledged!\n");
    close(c_sock);
    close(s_sock);
    return 0;
}
