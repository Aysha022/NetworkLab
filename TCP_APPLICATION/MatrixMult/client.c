#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

typedef struct {
    int n;
    int m;
    int arr[10][10];
} Matrix;

int main() {
    int PORT = 7777;
    struct sockaddr_in server;
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sock_fd, (struct sockaddr *) &server, sizeof server) != 0) {
        perror("CONNECT failed");
        return 1;
    }

    Matrix A;
    read(sock_fd, &A, sizeof A);

    printf("Matrix of size %d by %d\n", A.n, A.m);
    for (int i = 0; i < A.n; i++) {
        for (int j = 0; j < A.m; j++) {
            printf("%d ", A.arr[i][j]);
        }
        printf("\n");
    }

    close(sock_fd);
}
