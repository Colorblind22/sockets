#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 256

int main()
{
    char buffer[BUFFER_LENGTH],
    recieve[BUFFER_LENGTH];        
    fputs(">>",stdout);

    while(1)
    {
        fgets(recieve, BUFFER_LENGTH, stdin);
        printf("%s", recieve);
        fputs(">>",stdout);
        for(int i = 0; i < strlen(recieve)/2; i++)
        {
            char c = recieve[strlen(recieve)/2-1-i];
            fputc(c, stdout);
            ungetc(c, stdin);
        }
    }
}