#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_LENGTH 256
#define SERVER_PORT 56542
#define SERVER_NAME "name_of_server"


int main(int argc, char* argv[])
{
    struct sockaddr_in serveraddr;
    char server[BUFFER_LENGTH];
    
    // socket
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);

    if(argc > 1)
        strcpy(server, argv[1]);
    else
        strcpy(server, SERVER_NAME);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, server, &serveraddr.sin_addr.s_addr) != -1)
    {
        
    }

    // bind
    // connect

    // send
    // recv
    // close
    return 0;
}