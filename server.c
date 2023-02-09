#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>

#define BUFFER_LENGTH 256
#define PORT 8080

int main(int argc, char* argv[])
{
    typedef struct client_connection
    {
        char username[BUFFER_LENGTH];
        int socket;
    } client;

    int on=1, max_clients=64;
    int status, max_sd, addrlen, socket_descriptor, comm_socket_descriptor;
    struct sockaddr_in serveraddr;
    char 
    buffer[BUFFER_LENGTH], 
    msg[BUFFER_LENGTH],
    isolate_target[BUFFER_LENGTH],
    *token/* [BUFFER_LENGTH]*/,
    //hello[6] = "Hello",
    delim[2] = ":";
    client clients[max_clients];
    fd_set socket_set;

    
    char serverstr[7] = "Server";

    do
    {
        int k;
        for(k = 0; k < max_clients; k++)
        {
            clients[k].socket = 0;
        }
        
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
        status = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (char *)&on, sizeof(on));
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

        addrlen = sizeof(serveraddr);
        puts("Listening...");

        while(1)
        {
            FD_ZERO(&socket_set);
            FD_SET(socket_descriptor, &socket_set);
            max_sd = socket_descriptor;

            int i;
            int sd;
            for(i = 0; i < max_clients; i++)
            {
                sd = clients[i].socket;
                if(sd > 0)  // if valid sd
                    FD_SET(sd, &socket_set); // add to set
                if(sd > max_sd) // select highest sd for select()
                    max_sd = sd;
            }

            int activity = select(max_sd + 1, &socket_set, NULL, NULL, NULL);
            if((activity < 0) && (errno!=EINTR))
            {
                puts("select error");
            }

            if(FD_ISSET(socket_descriptor, &socket_set))
            {
                if((comm_socket_descriptor = accept(socket_descriptor, (struct sockaddr*)&serveraddr, (socklen_t *)&addrlen))<0)
                {
                    perror("accept() failed");
                }
                
                if(recv(comm_socket_descriptor, buffer, BUFFER_LENGTH, 0) <= 0)
                {
                    perror("recv() failed");
                }
                
                printf("New connection, username \"%s\", socket %d, ip %s, port %d\n", buffer, comm_socket_descriptor, inet_ntoa(serveraddr.sin_addr), ntohs(serveraddr.sin_port));
                
                /* int welcomelen = (strlen(buffer) + strlen(serverstr) + strlen(msg) + 1);
                snprintf(msg, welcomelen, "%s:%s : %s", buffer, serverstr, msg);
                status = send(comm_socket_descriptor, msg, welcomelen, 0);
                if (status <= 0)
                {
                    perror("send() failed");
                    break;
                }
                status = send(comm_socket_descriptor, msg, strlen(msg), 0) != strlen(msg);
                if (status <= 0)
                {
                    perror("send() failed");
                    break;
                }
                puts("sent welcome message"); */

                int j;
                for(j = 0; j < max_clients; j++)
                {
                    if(clients[j].socket == 0)
                    {
                        clients[j].socket = comm_socket_descriptor;
                        strcpy(clients[j].username, buffer);
                        //printf("clients[%d] = {username=\"%s\", socket=%d}\n", j, clients[j].username, clients[j].socket);
                        break;
                    }
                }
            }
            
            int valread;
            for(int k = 0; k < max_clients; k++)
            {
                sd = clients[k].socket;

                if(FD_ISSET(sd, &socket_set))
                {
                    if((valread = read(sd,buffer,256)) == 0)
                    {
                        printf("User %s at socket %d disconnected\n", clients[k].username, clients[k].socket);
                        close(sd);
                        clients[k].socket = 0;
                        strcpy(clients[k].username, "");
                    }
                    else 
                    {
                        /* given "target:from : content"
                            strtok ":" to isolate "target" part of message
                            iterate through clients[] and strcmp() for the match
                            send message trimmed to just "from : content" to user designated by "target" 
                        */
                        buffer[valread] = '\0';

                        // strtok ":" to isolate "target" part of message
                        strcpy(isolate_target, buffer);
                        strtok(isolate_target, delim); 

                        // iterate through clients[] and strcmp() for the match
                        client *target = NULL;
                        for(int index = 0; index < max_clients; index++)
                        {   
                            if(!strcmp(isolate_target, clients[index].username))
                            {
                                target = &clients[index];
                                break;
                            }
                        }
                        if(target == NULL)
                        {
                            perror("user not found");
                            send(clients[k].socket, "Error: targeted user does not exist", 36, 0);
                            break;
                        }

                        // send message trimmed to just "from : content" to user designated by "target"
                        token = strchr(buffer, ':');
                        token++;
                        strcpy(msg, token);
                        int message_length = (strlen(msg)+1);
                        status = send(target->socket, msg, message_length, 0);
                        if(status <= 0)
                        {
                            perror("send() failed");
                            break;
                        }
                        free(token);
                    }
                }
            }
        }
    } while(0);

    //close
    close(socket_descriptor);
    close(comm_socket_descriptor); // maybe register to atexit
    for(int i = 0; i < max_clients; i++)
        if(clients[i].socket != 0) close(clients[i].socket);  
      
    return 0;
}