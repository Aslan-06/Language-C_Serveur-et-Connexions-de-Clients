#include <netinet/in.h> //connect()
#include <sys/socket.h> //socket()
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "mesFonctions.h"

// das mesFonction.h nous avons structure User
// Au lancement du serveur, faut juste passer le port, le serveur s'installe sur l'adresse ip local

typedef struct
{
    int socket;
    User* users;
    int nbUsers;
} threadArgs;

User * loadLoginIP(char * fileName, int *nbUsers){
        //liste pour charger la liste les users existents sur le fichier realloc **

    FILE * fd = fopen(fileName, "r");

    if (fd==NULL)
        return NULL;

    User* users = (User*)malloc(sizeof(User));

    char * ligneLu = NULL;
    int compteur = 0;
    do{
        ligneLu = litLigne(fd);
        if(ligneLu != NULL && strlen(ligneLu) > 0){
            strcpy((users + compteur)->name, ligneLu);
            ligneLu = litLigne(fd);
            strcpy((users + compteur)->IP, ligneLu);
            compteur++;
            users = realloc(users, sizeof(User) * (compteur+1));
        }
    }while(ligneLu != NULL && strlen(ligneLu) > 0);
    * nbUsers = compteur;
    return users;
}


int checkLoginIP(User* user, User* loginIPs, int nbUsers){
    int compteur;
    for(compteur = 0; compteur < nbUsers; compteur++){
        printf("Client : %s, %s    |||   User courant : %s, %s\n", user->name, user->IP, (loginIPs + compteur)->name, (loginIPs + compteur)->IP);
        if( strcmp(user->name, (loginIPs + compteur)->name) == 0  && strcmp(user->IP,(loginIPs + compteur)->IP) == 0 ){
            printf("J'ai trouvé l'user dans le fichier\n");
            return 1;
        }
    }
    return 0;
}

void * communiquerAvecClient(void * args) {
    threadArgs * argsConverti = (threadArgs *) args;
    int sockClient = argsConverti->socket;
    User * usersReconnus = argsConverti->users;
    int nbUsersReconnus = argsConverti->nbUsers;
    User * userClient;
    char * messageARetourner;

    userClient = readStruct(sockClient);

    // Authentification du client
    if (checkLoginIP(userClient, usersReconnus, nbUsersReconnus)) {
        messageARetourner = "Connected, vous êtes authentifié";
    }
    else {
        messageARetourner = "Refused";
    }
    
    sendMess(sockClient, messageARetourner);
   
    close(sockClient);
}

void mainMultithreadServer(int port, char* loginFile) {
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    sa.sin_addr.s_addr = (long) inet_addr("127.0.0.1");

    int sockServeur = createServer(sa);
    printf("Serveur crée avec le socket %d\n", sockServeur);

    int* nbUsers = (int*)malloc(sizeof(int));
    User * usersReconnus = loadLoginIP(loginFile, nbUsers);

    while(1){
        pthread_t tid;
        threadArgs * args;

        int sockClient = createConnection(sockServeur,sa);    
        printf("Socket client = %d\n", sockClient);
        if (sockClient == -1){
            perror("Connexion refusÃ©e");
            exit(EXIT_FAILURE); 
        }

        args = (threadArgs *) malloc(sizeof(threadArgs));
        args->socket = sockClient;
        args->users = usersReconnus;
        args->nbUsers = *nbUsers;

        pthread_create(&tid, NULL, communiquerAvecClient, args);
        pthread_join(tid, NULL);        // Il ne pourra pas aller plus loin, le programme va revenir au accept. En faisant ca, on peut direct prendre un autre client après la connextion d'un client
        printf("%s\n", "Thread terminé");
    }
}

void main(int argc, char **argv){
    int port = atoi(argv[1]);       // >>>>>>>> Seulement utiliser 1 SEUL ARGUMENT : LE PORT !!! <<<<<<<<<<
    mainMultithreadServer(port, "usersLoginIp.txt");
}