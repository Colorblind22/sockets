#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "message.h"

#define BUFFER_LENGTH 256
#define SERVER_PORT 8080
#define SERVER_NAME "127.0.0.1"


int main(int argc, char* argv[])
{
    struct sockaddr_in serveraddr;
    message 
    *msg = (message*)malloc(sizeof(*msg)), 
    *recieve = (message*)malloc(sizeof(*recieve));
    char 
    *server,
    buffer[BUFFER_LENGTH];
    int socket_descriptor;

    do
    {   
        int status = 0;
        
        // socket
        socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if(socket_descriptor < 0)
        {
            perror("socket() failed");
            break;
        }
        printf("socket: %d\n", socket_descriptor);

        if(argc > 1)
            server = strdup(argv[1]);
        else
            server = strdup(SERVER_NAME);

        printf("server name: %s\n", server);

        puts("Enter a username:");
        fgets(buffer, BUFFER_LENGTH, stdin);
        printf("String read: %s\n" , buffer);
        msg->from = strdup(buffer);
        puts("Enter the username of target:");
        fgets(buffer, BUFFER_LENGTH, stdin);
        printf("String read: %s\n" , buffer);
        msg->to = strdup(buffer);

        // set up server info
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(SERVER_PORT);

        inet_pton(AF_INET, server, &serveraddr.sin_addr);
        
        // connect
        printf("connecting...");
        status = connect(socket_descriptor, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if(status < 0)
        {
            perror("connect() failed");
            break;
        }
        printf("\tconnected to server\n");

        printf("Enter message to send:\n");
        fgets(buffer, BUFFER_LENGTH, stdin);
        printf("String read: %s\n" , buffer);
        msg->content = strdup(buffer);

        // send
        status = send(socket_descriptor, &msg, sizeof(msg), 0);
        if(status <= 0)
        {
            perror("send() failed");
            break;
        }
        printf("send() : %d\n", status);
        // recv
        status = recv(socket_descriptor, recieve, 512, 0);
        if(status <= 0)
        {
            perror("recv() failed");
            break;
        }
        printf("recv() : %d\n", status);
        printf("message recieved: %s\n", recieve->content);
    } while(0);
    
    // close
    close(socket_descriptor);

    return 0;
}