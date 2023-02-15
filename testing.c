#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 256

int main()
{
    char buffer[BUFFER_LENGTH],
        recieve[BUFFER_LENGTH];
    fputs(">>", stdout);
    strcpy(recieve, "message being recieved");
    while (1)
    {
        fgets(recieve, BUFFER_LENGTH, stdin);
        recieve[strcspn(recieve, "\n")] = '\0';
        printf("\33[2K\r%s\n", recieve);
        for(int l = strlen(recieve)-1; l >= 0; l++)
        {
            ungetc(recieve[l], stdin);
        }
        for(int i = 0; i < strlen(recieve); i++)
        {
            fputc(recieve[i], stdout);
        }
    }
}