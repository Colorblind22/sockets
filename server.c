#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>
//#include <sys/types.h>

#define BUFFER_LENGTH 256

int main(int argc, char* argv[])
{
    int on=1;
    struct sockaddr_in serveraddr;
    
    //socket
        //referred to in guide as sockfd for socket file descriptor
    int socket_descriptor = socket(/*domain*/AF_INET, /*type*/SOCK_STREAM, /*protocol*/0);
    /*
        AF_INET specifies ipv4
        SOCK_STREAM specifies TCP
        0 is what it told me to use idk
            - $ man protocols
            - /etc/protocols
    */
    // setsockopt to help in reuse of address and port
    setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));

    //bind
    bind(socket_descriptor, (struct sockaddr *)&serveraddr, sizeof(serveraddr));


    //listen
    int cont = listen(socket_descriptor, 5);

    printf("Listening...\n");

    //accept
    int client_socket_descriptor = accept(socket_descriptor, NULL, NULL);

    struct pollfd fd;
    nfds_t n_fd = 1;
    // number of file descriptors
    int recieve = poll(&fd, n_fd, 30000);

    //recv
    //send
    //close
    return 0;
}