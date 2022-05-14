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
#define PORT 8080



//creating log file for server side
FILE *ptrLogs;

//structure defining accounts malloc
struct _account {
    char username[50];
    char password[50];
};

//function that reads config file
//https://stackoverflow.com/questions/14815449/read-name-and-password-from-a-file-in-c
int read_config(struct _account *users){
    char *filename = "config.txt";
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(ptrLogs, "Config.txt error.\n");
        fflush(ptrLogs);
    };

    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];
    int i = 0, firstLine = 0;

    while (fgets(buffer, MAX_LENGTH, fp)) {
        if (firstLine == 0) {
            firstLine = 1;
        } else {
            buffer[strcspn(buffer, "\n")] = 0;
            buffer[strcspn(buffer, "\r")] = 0;
            printf("%s", buffer);
            char* sp = ";";
            char* user = strtok(buffer, sp);
            char* password = strtok(NULL, sp);
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
    printf("Starting daemonize\n");
    daemonize();

    // SOCKET
    ptrLogs = fopen("serverlogs.txt", "w");



    int socket_desc , new_socket , c, data;
    struct sockaddr_in server , client;
    struct sockaddr_in address;
    char *message;
    int opt = 1; 
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    
    //Prepare the incoming port and address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8080 );
    
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("Bound");
    
    //Listen
    listen(socket_desc , 3);
    
    struct _account users[10] = {"", ""};
    read_config(users);


    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    { // GET OPTION
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
                    send(new_socket, "1", strlen("1"), 0);
                    break;
                }
                
            }
        } 

        if (new_socket < 0) {
            fprintf(ptrLogs, "Accept failed.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }
    
    
    return EXIT_SUCCESS;
}