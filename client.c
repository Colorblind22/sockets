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


int main(int argc, char* argv[])
{
    struct sockaddr_in serveraddr;

    char 
    *server,
    username[BUFFER_LENGTH], target[BUFFER_LENGTH],
    buffer[BUFFER_LENGTH], message[BUFFER_LENGTH]; // reject structs return to char[]
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

        if(argc > 1)
            server = strdup(argv[1]);
        else
            server = strdup(SERVER_NAME);


        puts("Enter a username:");
        fgets(buffer, BUFFER_LENGTH, stdin);
        strcpy(username, buffer);
        username[strcspn(username, "\n")] = '\0';
		
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
		
        status = send(socket_descriptor, username, sizeof(username), 0);
        if(status <= 0)
        {
            perror("send() failed");
            break;
        }

        puts("Enter a username to send messages to:");
        fgets(buffer, BUFFER_LENGTH, stdin);
        strcpy(target, buffer);
        target[strcspn(target, "\n")] = '\0';

        int leave = 1;
        int * die = &leave;

        pid_t pid = fork();
        if(pid < 0)
        {
            perror("fork() failed");
            break;
        }
        //printf("process id %d\n", pid);
		char keyword[6] = "^q";
        if(pid == 0)
        {    
            /* printf("\tpid %d recieving\n", pid); */
            do
            {
                // recv
                if(!*die) break;
                status = recv(socket_descriptor, buffer, BUFFER_LENGTH, 0);
                if (status <= 0)
                {
                    perror("recv() failed");
                    break;
                }
                buffer[status] = '\0';
                printf("%s\n", buffer);
            } while (*die);
            puts("closing recv() fork");
            break;
        }
        if(pid > 0)
        {
            /* printf("\tpid %d sending\n", pid); */
            do
            {
                printf("Enter message to send:\n");
                fgets(buffer, BUFFER_LENGTH, stdin);
                buffer[strcspn(buffer, "\n\uFFFd")] = '\0'; // $ man strcspn
                *die = strcmp(buffer, keyword);
                if(!*die) break;
                snprintf(message, (strlen(target)+strlen(username)+strlen(buffer)+4), "%s:%s : %s", target, username, buffer);
                status = send(socket_descriptor, message, sizeof(message), 0);
                if (status <= 0)
                {
                    perror("send() failed");
                    break;
                }
            } while (*die);
            puts("closing send() fork");
            break;
        }

    } while(0);
    
    // close
    close(socket_descriptor);

    return 0;
}