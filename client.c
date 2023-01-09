#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define BUFFER_LENGTH 256
#define SERVER_PORT 8080
#define SERVER_NAME "127.0.0.1"


void die(int sd)
{
    close(sd);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    struct sockaddr_in serveraddr;
    struct addrinfo hints, *res;
    char server[BUFFER_LENGTH];
    
    // socket
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    printf("sockfs: %d\n", socket_descriptor);

    if(argc > 1)
        strcpy(server, argv[1]);
    else
        strcpy(server, SERVER_NAME);

    printf("server name: %s\n", server);

    // set up server info
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);
    //if(
    inet_pton(AF_INET, server, &serveraddr.sin_addr);
    // != -1)
    /*{
        printf("retrieve ip of server...\n");
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_flags = AI_V4MAPPED;
        getaddrinfo(server, NULL, &hints, &res);

        memcpy
        (
            &serveraddr.sin_addr,
            (&((struct sockaddr_in *)(res->ai_addr))->sin_addr),
            sizeof(serveraddr.sin_addr)
        );

        freeaddrinfo(res);
        printf("server ip retrieved\n");
    }*/

    // bind
    // connect
    printf("connecting...\n");
    if(connect(socket_descriptor, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        perror("connect() failed");
        die(socket_descriptor);
    }
    printf("connected\n");

    // send
    // recv
    // close
    return 0;
}