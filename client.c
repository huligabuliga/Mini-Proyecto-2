#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
//defining port
#define PORT 8080
//defining buffer size for buffer in bytes
//"BUFSIZE should be equal to the size of your buffer in bytes. read() will stop when buffer is full" 
#define BUFFER_SIZE 1024

int main(int argc , char *argv[])
{
    int socket_desc;
    int s = 0;
    int validateRead;
    struct sockaddr_in server;
    char *message;

    char mybuffer[BUFFER_SIZE];
    
    //username password up to 50 characters
    char username[50], password[50];

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
        
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( PORT );

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("connect error");
        return 1;
    }
    
    puts("Connected\n");

    // ask for user and password
    printf("Please enter username and password to authorize connection\n");
    printf("username: ");
    scanf("%s", username);
    printf("password: ");
    scanf("%s", password);
    //send it to message in username;password format according to config file
    strcat(message,"0;");
    strcat(message,username);
    strcat(message,";");
    strcat(message,password);
    strcat(message,";");

    

    //Send message (user) data
    if( send(s , message , strlen(message) , 0) < 0)
    {
        puts("Send failed");
        return 1; //exit
    }

    //validate data from server
    if( (validateRead = read(s, mybuffer, 1024) ) < 0)
	{
		puts("validation failed.");
	}

    printf("%s", mybuffer);

    if(atoi(mybuffer) == 1)
    {
        puts("user data correct, connected");
    } else {
        puts("username or password incorrect");
        return 1; //exit
    }

    
    
    return 0;
}