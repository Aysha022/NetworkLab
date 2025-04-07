#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 8900
#define MAX 1000
#define SA struct sockaddr
#define SAI struct sockaddr_in

int main(){
        int sockfd,n=0;
        SAI server;
        char buff[MAX];

        sockfd=socket(AF_INET,SOCK_STREAM,0);

        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.s_addr = htonl(INADDR_ANY);

        //bzero(buffer,MAX);

        if((connect(sockfd,(SA*)&server,sizeof(server))) == 0) {
          printf("Connected with server..\n");
        }

        bzero(buff,MAX);
        printf("Enter the number of messages : ");
        while((buff[n++] = getchar()) != '\n');
        write(sockfd,buff,sizeof(buff));
        int no = atoi(buff);
        int i = 1;

        while(1) {
                bzero(buff,MAX);
                printf("Enter the data : ");
                n = 0;
                while((buff[n++] = getchar()) != '\n');
                write(sockfd,buff,sizeof(buff));
                printf("The message is %s\n",buff);

                bzero(buff,MAX);
                read(sockfd,buff,sizeof(buff));
                printf("The reply is %s\n",buff);

                if(strcmp("ack",buff) == 0) {
                   i++;
                }

                if(i == no) {
                   break;
                }
        }
        close(sockfd);
}
