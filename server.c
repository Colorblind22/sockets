#include <stdio.h>
//#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <unistd.h>
//#include <sys/types.h>
#include "message.h"

#define BUFFER_LENGTH 256
#define PORT 8080

int main(int argc, char* argv[])
{
    int on=1;
    int status, socket_descriptor, comm_socket_descriptor;
    struct sockaddr_in serveraddr;
    char buffer[BUFFER_LENGTH];

    typedef struct client
    {
        char username[64];
        int socket;
    };
    

    do
    {
        /*socket
            referred to in guide as sockfd for socket file descriptor
        
            AF_INET specifies ipv4
            SOCK_STREAM specifies TCP
            0 is what it told me to use idk
                - $ man protocols
                - /etc/protocols
        */
        socket_descriptor = socket(/*domain*/AF_INET, /*type*/SOCK_STREAM, /*protocol*/0);
        if(socket_descriptor < 0)
        {
            perror("socket() failed");
            break;
        }

        // setsockopt to help in reuse of address and port
        status = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on));
        if(status < 0)
        {
            perror("setsockopt() failed");
            break;
        }
        
        //bind
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = INADDR_ANY;
        serveraddr.sin_port = htons(PORT);
        status = bind(socket_descriptor, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
        if(status < 0)
        {
            perror("bind() failed");
            break;
        }

        //listen
        status = listen(socket_descriptor, 5);
        if(status < 0)
        {
            perror("listen() failed");
            break;
        }
        printf("Listening...\n");

        //accept
        comm_socket_descriptor = accept(socket_descriptor, NULL, NULL);
        if(comm_socket_descriptor < 0)
        {
            perror("socket() failed");
            break;
        }

        //poll
        struct pollfd fd;
        // number of file descriptors
        nfds_t n_fd = 1;
        status = poll(&fd, n_fd, 30000);
        if(status == 0)
        {
            perror("poll() timed out");
            break;
        }
        else if(status < 0)
        {
            perror("poll() failed");
        }

        //recv
        status = recv(comm_socket_descriptor, buffer, sizeof(buffer), 0);
        if(status < 0)
        {
            perror("recv() failed");
        }
        printf("message recieved: %s\n", buffer);
        //send
        printf("echoing message back to client\n");
        status = send(comm_socket_descriptor, buffer, sizeof(buffer), 0);
        if(status < 0)
        {
            perror("send() failed");
        }
    } while(0);

    //close
    close(socket_descriptor);
    close(comm_socket_descriptor);

    return 0;
}