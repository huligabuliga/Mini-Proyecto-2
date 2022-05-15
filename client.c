#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
//defining port to connect
#define PORT 8080
 
int main(int argc, char const* argv[])
{
    int s = 0;
    int validateRead;
    struct sockaddr_in server;

    char *message, buffer[1024] = { 0 };


    char username[20], password[20];

    // CREATE
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        puts("Socket creation error.");
        return -1;
    }
    puts("Socket created.");

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {
        puts("Invalid addres.");
        return -1;
    }
 
    // CONNECT
    if (connect(s, (struct sockaddr*)&server,sizeof(server)) < 0) {
        puts("Connection failed.");
        return -1;
    }

    // VALIDATE CREDENTIALS
    printf("Please enter username and password to connect\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    strcat(message,"0;");
    strcat(message,username);
    strcat(message,";");
    strcat(message,password);
    strcat(message,";");

    // Send user data.
    if (send(s, message, strlen(message), 0) < 0) {
		puts("Send failed.");
		return 1;
	}

    // Receive confirmation on whether or not the user is valid.
    if ((validateRead = read(s, buffer, 1024)) < 0)
	{
		puts("Valread failed.");
	}

    printf("%s", buffer);

    
    if (atoi(buffer) == 1) {
        puts("Connected.");
        // insert quries here: SELECT, INSERT, JOIN.

    } else {
        puts("Invalid credentials.");
        return 1;
    }

    close(s);
    return 0;
}