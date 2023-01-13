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
    struct message msg, recieve;
    char server[BUFFER_LENGTH];
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
        //printf("socket: %d\n", socket_descriptor);

        if(argc > 1)
            strcpy(server, argv[1]);
        else
            strcpy(server, SERVER_NAME);

        //printf("server name: %s\n", server);

        puts("Enter a username:");
        fgets(msg.from, sizeof(msg.from), stdin);
        puts("Enter the username of target:");
        fgets(msg.to, sizeof(msg.to), stdin);

        // set up server info
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(SERVER_PORT);

        inet_pton(AF_INET, server, &serveraddr.sin_addr);
        
        // connect
        //printf("connecting...");
        status = connect(socket_descriptor, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if(status < 0)
        {
            perror("connect() failed");
            break;
        }
        printf("\tconnected\n");

        printf("Enter message to send:\n");
        fgets(msg.content, sizeof(msg.content), stdin);

        // send
        send(socket_descriptor, &msg, sizeof(msg), 0);
        // recv
        recv(socket_descriptor, &recieve, sizeof(recieve), 0);
        printf("message recieved: %s\n", recieve.content);
    } while(0);
    
    // close
    close(socket_descriptor);

    return 0;
}