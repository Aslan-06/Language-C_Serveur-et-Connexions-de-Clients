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

    //3 Echange serveur et client
    char * message =(char*)argv[3];
    sendMess(socketServer, message);
    char * messageRecu = readMess(socketServer);

    printf("Message du serveur : %s\n", messageRecu);    

    close(socketServer);
    
    return 0;
}