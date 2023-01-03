#include <netinet/in.h> //connect()
#include <sys/socket.h> //socket()
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "mesFonctions.h"
#include <pthread.h>

void * communiquerAvecClient(void * arg) {
    
    int * sockClient = (int *) arg;

    //3-Echange serveur et client
    char * message = readMess(*sockClient);
    // printf("Message du client : %s\n", message);
    printf("%s%s\n", "message recu : ", message);
    sendMess(*sockClient, message);
   
    close(*sockClient);
}

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
        printf("%s\n", "nouvelle boucle");
        pthread_t tid;

        int sockClient = createConnection(sockServeur,sa);    
        printf("Socket client = %d\n", sockClient);
        if (sockClient == -1){
            perror("Connexion refusÃ©e");
            exit(EXIT_FAILURE); 
        }

        pthread_create(&tid, NULL, communiquerAvecClient, &sockClient);
        pthread_join(tid, NULL);        // Il ne pourra pas aller plus loin, le programme va revenir au accept
        printf("%s\n", "Thread terminé");
    }
    return 0;
}
