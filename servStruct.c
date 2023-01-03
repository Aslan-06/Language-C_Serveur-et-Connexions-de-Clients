#include <netinet/in.h> //connect()
#include <sys/socket.h> //socket()
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mesFonctions.h"

int main(int argc, char **argv){

    // gcc -c mesFonctions.c
    // gcc serv.c -o serv mesFonctions.o
    // ./serv ifconfig 8080 
    //char * ip = (char *)argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = (long)inet_addr(argv[1]);
    int sockServeur = createServer(sa);
    printf("Serveur crée avec le socket %d\n", sockServeur);

    while(1){
        int sockClient = createConnection(sockServeur,sa);
        printf("Socket client = %d\n", sockClient);

        if (sockClient == -1){
            perror("Connexion refusée");
            exit(EXIT_FAILURE); 
        }

        //3-Echange serveur et client
        User * userStruct = readStruct(sockClient);
        
        printf("le nom du client : %s\n",userStruct->name);    
        printf("IP du client : %s\n",userStruct->IP);

	    sendStruct(sockClient, userStruct);
       
        close(sockClient);

    }
    return 0;
}
