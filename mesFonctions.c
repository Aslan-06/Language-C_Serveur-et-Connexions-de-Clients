#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "mesFonctions.h"
 
int sendMess(int sockId, char* query) {
    int envoie = send(sockId, query, strlen(query)+1, 0);
    if(envoie == -1) {
        perror("Erreur d'envoie de message\n");
        return -1;
    }
    return envoie; 
}

char* readMess(int sockId) {
    char* message = (char*)malloc(sizeof(char));
    char c = 1;
    int i = 0;
    while(c != '\0') {
        recv(sockId, &c, 1, 0);
        *(message + i) = c;
        i++;
        message = realloc(message, (i+1)*sizeof(char));
    }
    *(message + i) = '\0';
    return message;
}

/** les méthodes liées au client */
/*Connexion du client sur le serveur */
int connectToServer(char* ip, int port) {
    struct sockaddr_in sa;
    sa.sin_family = AF_INET;
sa.sin_port = htons(port); //port du serveur
sa.sin_addr.s_addr = (long)inet_addr(ip);

//Creation Socket
int sd = socket(PF_INET, SOCK_STREAM, 0);
if(sd == -1) {
    printf("Erreur de création de Socket\n");
    return -1;
}

//Connexion
int connexion = connect(sd, (struct sockaddr *)&sa, sizeof(sa));
if(connexion == -1) {
    perror("Erreur de connexion\n");
    return -1;
}

return sd;

}
/** les méthodes liées au serveur */

/*création du serveur */

int createServer(struct sockaddr_in sa){
    int sd=socket(PF_INET, SOCK_STREAM, 0);
    int err=bind(sd, (struct sockaddr *)&sa, sizeof(struct sockaddr));
    if(err == -1){
        perror("--->problème de Bind....");
        return -1;
    }
    err=listen(sd,5);
    if(err < 0){
        perror("--->problème de Listen....");
        return -1;
    }
    return sd;
}

/*lancement du serveur */

int createConnection(int serverSocket, struct sockaddr_in sa){
    printf("En attente de connexion...\n\n");
    int s = sizeof(struct sockaddr);
    
    int sockClient = accept(serverSocket, (struct sockaddr*)&sa, (socklen_t*)&s);
    if(sockClient == -1){
        perror("Erreur de connexion\n");
        return -1;
    }
    
    return sockClient;
}


// avec les structures

int sendStruct (int comSocket, User * aUser){
    // printf("user name %s\n", aUser->name);
    int nbs1 = sendMess(comSocket, aUser->name);
    printf("octets envoyes name: %d\n", nbs1);
    int nbs2 = sendMess(comSocket, aUser->IP);
    printf("octets envoyes ip: %d\n", nbs2);
    return nbs1+nbs2;
}

User * readStruct (int socket){
    User * buffer = (User*)malloc(sizeof(User));
    strcpy(buffer->name, readMess(socket));
    strcpy(buffer->IP, readMess(socket));   
    return buffer;
}

FILE * ouvrirFichier(char * nom, char * mode){

   FILE * pf=NULL;

    if ((strcmp(mode,"r")==0 || strcmp(mode,"w")==0 || strcmp(mode,"r+")==0 || strcmp(mode,"w+")==0 || strcmp(mode,"a")==0||strcmp(mode,"a+")==0) && strlen(nom) >0)

    {
        FILE * pf=fopen(nom,mode);
        if (pf!=NULL){
            
            return pf;
            
        }
    }

return pf;
}
/**
cette méthode ferme le fichier, s'il est valide
*/
void fermerFichier(FILE * pf){
    
    if (pf!=NULL) fclose(pf);
}
char * litLigne(FILE * fd){
	if (fd==NULL)
	{
		perror("fichier vide");
		return NULL;
	}
	char * ligne =(char *)malloc(sizeof(char));
	char c=0;
	int i=0;
	do 
    	{
		c=fgetc(fd);
    		*(ligne+i) = c;
        	ligne= realloc(ligne, sizeof(char*)*(i+1));
		i++;
    	} while(c!='\n' && !feof(fd));
	*(ligne+i-1)=0;
	return ligne;
}

char * litFichier(char * nomFich){
    
    //test si fichier est vide 
    FILE* fd = fopen(nomFich, "r");
    if (fd==NULL)
    {
        perror("fichier vide");
        return NULL;
    }
    char * fichierLu = (char *) malloc(sizeof(char));
    int i=0;
    do 
        {
            *(fichierLu+i) = fgetc(fd);
            fichierLu= realloc(fichierLu, sizeof(char*)*(i+1));
        i++;
        } while(!feof(fd));
    
    fclose(fd);
    free(fd);
    *(fichierLu+i-1)=0;
    return fichierLu;
}

