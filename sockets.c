#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    char *mode;
    if(argv[1]) mode = argv[1];
    if(strcmp(mode, "server") == 0)
    {
        printf("Server\n");
    }
    else if(strcmp(mode, "client") == 0)
    {
        printf("Client\n");
    }
    else
    {
        printf("Specify server or client with argv[1]\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}