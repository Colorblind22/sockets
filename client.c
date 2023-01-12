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

struct connection
{
    int socket_descriptor;
};

struct connection con;

/* sends message through socket and returns number of bytes sent */
int send_message(char *message)
{
    return ;
}

/* puts recieved message into recieve and returns number of bytes recieved */
int recieve_message(char *recieve)
{
    return recv(con.socket_descriptor, recieve, sizeof(recieve), 0);
}

int main(int argc, char* argv[])
{
    struct sockaddr_in serveraddr;
    char server[BUFFER_LENGTH], message[BUFFER_LENGTH], recieve[BUFFER_LENGTH];

    do
    {   
        int status = 0;
        
        // socket
        con.socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if(con.socket_descriptor < 0)
        {
            perror("socket() failed");
            break;
        }
        printf("socket: %d\n", con.socket_descriptor);

        if(argc > 1)
            strcpy(server, argv[1]);
        else
            strcpy(server, SERVER_NAME);

        printf("server name: %s\n", server);

        // set up server info
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_port = htons(SERVER_PORT);

        inet_pton(AF_INET, server, &serveraddr.sin_addr);
        
        // connect
        printf("connecting...");
        status = connect(con.socket_descriptor, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if(status < 0)
        {
            perror("connect() failed");
            break;
        }
        printf("\tconnected\n");

        printf("Enter message to send:\n");
        fgets(message, sizeof(message), stdin);

        // send
        send(con.socket_descriptor, message, sizeof(message), 0);
        // recv
        recieve_message(recieve);
        printf("message recieved: %s\n", recieve);
    } while(0);
    
    // close
    close(con.socket_descriptor);

    return 0;
}