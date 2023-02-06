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
    buffer[BUFFER_LENGTH], message[BUFFER_LENGTH];
    //delim[2] = ":"; // reject structs return to char[]
    int socket_descriptor;

    do
    {   
        int status = 0;
        int valread;
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
		
        status = send(socket_descriptor, username, strlen(username) + 1, 0);
        if(status <= 0)
        {
            perror("send() failed");
            break;
        }

        /*valread = recv(socket_descriptor, buffer, BUFFER_LENGTH, 0); 
        if (valread < 0)
        {
            perror("recv() failed");
            break;
        }
        buffer[valread] = '\0'; // trim string properly
        char * ret = strchr(buffer , ':');
        ret = strchr(ret, ret[1]);
        printf("%s\n", ret);
        free(ret); */


        sleep(1);
        puts("Enter a username to send messages to:");
        fgets(buffer, BUFFER_LENGTH, stdin);
        strcpy(target, buffer);
        target[strcspn(target, "\n")] = '\0';

        int leave = 1;
        int * die = &leave; // pointer so we can reference on both forks

        pid_t pid = fork(); // 0 is child, nonzero is parent
        if(pid < 0)
        {
            perror("fork() failed");
            break;
        }
		char keyword[6] = "leave";
        if(pid == 0)
        {    
            /* printf("\tpid %d recieving\n", pid); */
            do
            {
                // recv
                if(!*die) break;
                valread = recv(socket_descriptor, buffer, BUFFER_LENGTH, 0); 
                if (valread < 0)
                {
                    perror("recv() failed");
                    break;
                }
                else if (valread == 0)
                {
                    perror("server closed");
                }
                buffer[valread] = '\0'; // trim string properly
                char * ret = strchr(buffer , ':');
                ret = strchr(ret, ret[1]);
                printf("%s\n", ret);
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
                buffer[strcspn(buffer, "\n")] = '\0'; // $ man strcspn
                *die = strcmp(buffer, keyword);
                if(!*die) break;
                int message_length = (strlen(target)+strlen(username)+strlen(buffer)+6);
                snprintf(message, message_length, "%s:%s : %s", target, username, buffer);
                //message[0] = *((char *) &message_length);
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
    
    // works but TODO close forks

    // close
    close(socket_descriptor);
    exit(EXIT_SUCCESS);

    return 0;
}