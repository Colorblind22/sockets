#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <unistd.h>
//#include <sys/types.h>

#define BUFFER_LENGTH 256
#define PORT 8080

int main(int argc, char* argv[])
{
    int on=1;
    struct sockaddr_in serveraddr;
    
    /*socket
        referred to in guide as sockfd for socket file descriptor
    
        AF_INET specifies ipv4
        SOCK_STREAM specifies TCP
        0 is what it told me to use idk
            - $ man protocols
            - /etc/protocols
    */
    int socket_descriptor = socket(/*domain*/AF_INET, /*type*/SOCK_STREAM, /*protocol*/0);
    printf("sockfs: %d\n", socket_descriptor);
    // setsockopt to help in reuse of address and port
    printf("setsockopt: %d\n", setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on)));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(PORT);

    //bind
    printf("bind: %d\n", bind(socket_descriptor, (struct sockaddr *)&serveraddr, sizeof(serveraddr)));


    //listen
    int cont = listen(socket_descriptor, 5);
    printf("cont: %d\n", cont);
    printf("Listening...\n");
    //accept
    int comm_socket_descriptor = accept(socket_descriptor, NULL, NULL);
    printf("accept(): %d\n", comm_socket_descriptor);
    struct pollfd fd;
    // number of file descriptors
    nfds_t n_fd = 1;
    int recieve = poll(&fd, n_fd, 30000);
    printf("poll(): %d\n", recieve);

    //recv
    //send
    //close
    return 0;
}