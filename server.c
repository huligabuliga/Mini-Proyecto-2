#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
//define port

//before testing make sure port is free 

//fuser -n tcp -k 8080
#define PORT 8080
FILE *serverlogs;



//creating log file for server side
FILE *ptrLogs;

//structure defining accounts malloc
struct _account {
    char username[20];
    char password[20];
};

struct alumno{
    int matricula;
    char nombre[25];
    char apellido[25];
};

struct carrera{
    double calificacion;
    char carrera[50];
    char semestre[15];
};

int leeAlumno(struct alumno *alum){
    char str[200];
    int NumAlum = 0;
    FILE *archivo = fopen("table_1.txt", "r");

    if(archivo == NULL){
        fprintf(serverlogs, "Error abriendo tabla1.txt.\n");
        }
    
    while(fgets(str, sizeof(str), archivo)){
        int i = 0;
        int rec = 0;

        while(NumAlum!=0&&rec<5){
            
            char line[200] = {0};
            char *tok = strtok(str, ",");
            strcpy(line, tok);
            
        while(tok!=NULL&&rec==0){
            if(i==0){
                alum[NumAlum-1].matricula=atoi(line);
                i++;
            }
            else if(i==1){
                strcpy(alum[NumAlum-1].nombre, line);
                i++;
            }
            else if(i==2){
                strcpy(alum[NumAlum-1].apellido, line);
                rec = 1;
                i++;
            }
            i = 0;
            rec++;
            if(rec!=0){
                tok=strtok(NULL, ",");
                strcpy(str, tok);
            }
        }
    }
    NumAlum++;
        }
    fclose(archivo);
    return NumAlum;
}

int leeCarrera(struct carrera *car){
    char str[200];
    int NumCar = 0;
    FILE *archivo = fopen("table_2.txt", "r");

    if(archivo == NULL)
      fprintf(serverlogs, "Error abriendo tabla2.txt.\n");
    
    while(fgets(str, sizeof(str), archivo)){
        int i = 0;
        int rec = 0;

        while(NumCar!=0&&rec<5){
            
            char line[200];
            char *tok = strtok(str, ",");
            strcpy(str, tok);
            
        while(tok!=NULL&&rec==0){
            if(i==0){
                car[NumCar-1].calificacion = atof(line);
                i++;
           
            }
            else if(i==1){
                strcpy(car[NumCar-1].carrera, line);
                i++;
          
            }
            else if(i==2){
                strcpy(car[NumCar-1].semestre, line);
                i++;
         
            }
            i = 0;
            rec++;
            if(rec!=0){
                tok=strtok(NULL, ",");
                strcpy(str, tok);
            }
        }
    }
    NumCar++;
        }
    fclose(archivo);
    return NumCar;
}

//change this
//function that reads config file
//https://stackoverflow.com/questions/14815449/read-name-and-password-from-a-file-in-c
int read_config(struct _account *users){
    FILE *fp = fopen("config.txt", "r");

    char configBuffer[256];
    int i = 0;
    int fl = 0;

    while (fgets(configBuffer, 256, fp)) {
        if (fl == 0) {
            fl = 1;
        } 
        else 
        {
            configBuffer[strcspn(configBuffer, "\n")] = 0;
            configBuffer[strcspn(configBuffer, "\r")] = 0;
            printf("%s", configBuffer);
            char* sp = ",";
            char* user = strtok(configBuffer, sp);
            char* password = strtok(NULL, sp);
            //store in array
            strcpy(users[i].username, user);
            strcpy(users[i].password, password);
            i++;
        }
    }

    fclose(fp);

    return 0;
}

//adding deamon server from class
static void daemonize()
{
    pid_t pid;
    
    
     /* Fork off the parent process */
    pid = fork();
    
      /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
       /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    
    /* Catch, ignore and handle signals */
    /*TODO: Implement a working signal handler */
    //signal(SIGCHLD, SIG_IGN);
    //signal(SIGHUP, SIG_IGN);
    
    /* Fork off for the second time*/
    pid = fork();

    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* Set new file permissions */
    umask(027);
    
    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("./ServerData");
    
    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
    
    /* Open the log file */
    openlog("firstdeamon", LOG_PID, LOG_DAEMON);
    closelog();
}

int main(int argc , char *argv[])
{
   // START DAEMON
    printf("Starting Daemon-Server\n");
    daemonize();

    // SOCKET
    ptrLogs = fopen("serverlogs.txt", "w");

    int s, c, new_socket, data;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    // char buffer[3][20];
   
    // CREATE SOCKET
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        fprintf(ptrLogs, "Error creating socket.\n");
        fflush(ptrLogs);
        exit(EXIT_FAILURE);
    }
    fprintf(ptrLogs, "Socket has been created.\n");
    fflush(ptrLogs);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    // BIND SOCKET
    if (bind(s, (struct sockaddr*)&address, sizeof(address)) < 0) {
        fprintf(ptrLogs, "Bind failed.\n");
        fflush(ptrLogs);
        exit(EXIT_FAILURE);
    }
    fprintf(ptrLogs, "Bound.\n");
    fflush(ptrLogs);

    // LISTEN FOR CONNECTIONS
    listen(s, 3);
    fprintf(ptrLogs, "Waiting for connections...\n");
    fflush(ptrLogs);

    // GET CONFIG DATA
    struct _account users[10] = {"", ""};
    read_config(users);

//here
    struct alumno alum[50] = {0, "", ""}; //null before but breaks code... 
    struct carrera car[50] = {0, "", ""};
    int NumAlum = leeAlumno(alum)-1;
    int NumCar = leeCarrera(car)-1;

    // ACCEPT CONNECTIONS
    while ((new_socket = accept(s, (struct sockaddr*)&address, (socklen_t*)&addrlen))) {
        // GET OPTION
        data = read(new_socket, buffer, 1024);
        char tempBuffer[1024] = {0};
        strcpy(tempBuffer, buffer);

        char* sp = ";";
        char* option = strtok(tempBuffer, sp);
        
        if(atoi(option) == 0) {
            char* user = strtok(NULL, sp);
            char* password = strtok(NULL, sp);

            for (int i = 0; i < 10; i++) {
                int validUsername = strcmp(users[i].username, user);
                int validPassword = strcmp(users[i].password, password);

                fprintf(ptrLogs, "User Password %s %s\n", users[i].username, users[i].password);
                fflush(ptrLogs);
                fprintf(ptrLogs, "User Password %d %d\n", validUsername, validPassword);
                
                if(validUsername == 0 && validPassword == 0) {
                    fprintf(ptrLogs, "Connected, user authenticated.\n");            
                    fflush(ptrLogs);
                    send(new_socket, " 1 ", strlen(" 1 "), 0);
                    break;
                }
                
            }
        } 

        if (new_socket < 0) {
            fprintf(ptrLogs, "Accept failed.\n");
            exit(EXIT_FAILURE);
        }
    };

    fclose(ptrLogs);
    close(s);

    return EXIT_SUCCESS;

}