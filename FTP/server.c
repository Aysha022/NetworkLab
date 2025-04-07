#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>


#define MAX 80
#define PORT 8900
#define SA struct sockaddr
#define SAI struct sockaddr_in

void reverse(char *s) {
  int len = strlen(s);

  for(int i = 0; i < len/2; i++) {
     char temp = s[i];
     s[i] = s[len - i - 1];
     s[len - i - 1] = temp;
  }
  printf("%s\n",s);

}

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buff[MAX];


  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buff, MAX, 0);
    if (n <= 0){
      break;
      return;
    }
    buff[n] = '\0';
    reverse(buff);
    fprintf(fp, "%s\n", buff);
    bzero(buff, MAX);
  }
}

int main(){
    SAI server,client;
    int sockfd, connfd, len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket successfully created.....\n");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    if ((bind(sockfd, (SA*)&server, sizeof(server))) == 0)
        printf("Socket successfully binded..\n");

    if ((listen(sockfd, 5)) == 0)
        printf("Server listening..\n");

    len = sizeof(client);
    connfd = accept(sockfd, (SA*)&client, (socklen_t*)&len);
    printf("Server accept the client...\n");

    write_file(connfd);
    close(sockfd);
    return 0;
}
