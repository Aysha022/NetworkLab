#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int bucketSize,outputRate;

void leakybucket(int packets[],int n) {
        int bucket = 0;

        printf("\nTime Step | Incoming | Bucket Size | Sent | Remaining\n");
        printf("------------------------------------------------------\n");

        for(int i = 0; i < n; i++) {
                printf("%-9d | %-8d | ",i+1,packets[i]);

                if(packets[i] + bucket > bucketSize) {
                        printf("overflow!   | ");
                        bucket = bucketSize;
                }
                else {
                        bucket += packets[i];
                        printf("%-11d | ",bucket);
                }
                int sent = (bucket < outputRate)? bucket : outputRate;
                bucket -= sent;
                printf("%-4d | %-8d\n",sent,bucket);
                sleep(1);
        }
        while(bucket > 0 ) {
                int sent = (bucket < outputRate)? bucket : outputRate;
                bucket -= sent;
                printf("%-9s | %-8s | %-11d | %-4d | %-8d\n","Drain","--",bucket+sent,sent,bucket);
                sleep(1);
        }

}

int main() {
        int n;
        printf("Enter the bucket size : ");
        scanf("%d",&bucketSize);

        printf("Enter output rate : ");
        scanf("%d",&outputRate);

        printf("Enter the number of packets : ");
        scanf("%d",&n);

        int packets[n];
        printf("Enter the incoming packets : \n");
        for(int i = 0; i < n; i++) {
                scanf("%d",&packets[i]);
        }

        leakybucket(packets,n);
        return 0;
}
