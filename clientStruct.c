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
    // gcc client.c -o client mesFonctions.o
    // ./client ifconfig 8080 

    //Exercice 1
    char * ip = (char *)argv[1];
    int port = atoi(argv[2]);
    int socketServer = connectToServer(ip,port);
    printf("Connexion au serveur reussi (Socket=%d)\n", socketServer);

    //Exercice 2
    if(socketServer==-1){
        return 0;
    }

    //3 Authentification du client
    User * userClient = (User *) malloc(sizeof(User));
    strcpy(userClient->name, "Omar");
    strcpy(userClient->IP, "192.168.2.140");

    sendStruct(socketServer, userClient);
    

    printf("Message du serveur : %s\n", readMess(socketServer));    
    close(socketServer);
   
    
    /*
    //char * requete = (char *)argv[3];
    */
    
    return 0;
}

