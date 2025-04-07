#include <iso646.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct {
    int n;
    int m;
    int arr[10][10];
} Matrix;

int main() {
    struct sockaddr_in server, client;
    int PORT = 7777, len;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    bind(sock_fd, (struct sockaddr *) &server, sizeof server);

    printf("Server listening on PORT: %d\n", PORT);

    listen(sock_fd, 5);

    len = sizeof client;
    int conn_fd = accept(sock_fd, (struct sockaddr *) &client, (socklen_t *) &len);


    Matrix A;
    printf("Enter n m: ");
    scanf(" %d %d", &A.n, &A.m);
    printf("Enter matrix: ");
    for (int i = 0; i < A.n; i++) {
        for (int j = 0; j < A.m; j++) {
            scanf(" %d", &A.arr[i][j]);
        }
    }

    printf("Matrix of size %d by %d\n", A.n, A.m);
    for (int i = 0; i < A.n; i++) {
        for (int j = 0; j < A.m; j++) {
            printf("%d ", A.arr[i][j]);
        }
        printf("\n");
    }

    write(conn_fd, &A, sizeof A);

    close(sock_fd);
}
