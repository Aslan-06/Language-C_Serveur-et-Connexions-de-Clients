// avec les structures  f

#define STRING_SIZE 20

typedef struct userStruct{
    char name[STRING_SIZE];
    char IP[STRING_SIZE];
}User;

/*typedef struct{
char * name;
char * ip;
} user_1;*/

int connectToServer(char* IP, int port);

int sendMess(int socket, char* query);

int sendMessAuClient(int socket, char* query);

char * readMess(int socket);

char * readMessFromServer(int socket);

int createServer(struct sockaddr_in sa);

int createConnection(int serverSocket, struct sockaddr_in sa);


int sendStruct (int comSocket, User * aUser);

User * readStruct (int socket);


FILE * ouvrirFichier(char * nom, char * mode);


void fermerFichier(FILE * pf);



char * litFichier(char * nomFich);

char * litLigne(FILE * fd);