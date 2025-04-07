#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAX 1000
#define PORT 8900
#define SA struct sockaddr
#define SAI struct sockaddr_in

int main() {
        int sockfd,connfd,len,n,count=0;
        SAI server,client;
        char buff[MAX],buff1[MAX] = "ack",buff2[MAX] = "no";

        sockfd = socket(AF_INET,SOCK_STREAM,0);

        server.sin_family = AF_INET;
        server.sin_port = htons(PORT);
        server.sin_addr.s_addr = htonl(INADDR_ANY);

        bind(sockfd,(SA*)&server,sizeof(server));

        listen(sockfd,5);

        len = sizeof(client);
        connfd = accept(sockfd,(SA*)&client,(socklen_t*)&len);
        if(connfd == 0) {
                printf("Reciever accept the sender\n");
        }

        bzero(buff,MAX);
        read(connfd,buff,sizeof(buff));
        printf("The number is recieved\n");
        int no = atoi(buff);
        int i = 1;

        while(1) {
                bzero(buff,MAX);
                read(connfd,buff,sizeof(buff));
                printf("The message is %s\n",buff);

                if((strcmp("3\n",buff) == 0) || (strcmp("5\n",buff) == 0)){
                   if(count == 0 ){
                        sleep(3);
                        write(connfd,buff2,sizeof(buff2));
                        printf("The acknowledgment is %s\n",buff2);
                        count = 1;
                   }
                   else {
                        i++;
                        write(connfd,buff1,sizeof(buff1));
                        printf("The acknowledgement is %s\n",buff1);
                        count = 0;
                   }
                }
                else {
                        write(connfd,buff1,sizeof(buff1));
                        printf("The acknowledgement is %s\n",buff1);
                        i++;

                        if(i == no){
                           break;
                        }
                }
                if(i == no) {
                        break;
                }
        }
        close(sockfd);

}
