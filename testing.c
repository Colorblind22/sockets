#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGTH 256

int main()
{
    char *str = malloc(BUFFER_LENGTH), *rec = malloc(64);
    fputs(">>", stdout);
    int x = 1;
    do
    {
        fgets(str, sizeof(str), stdin);
        int c = strlen(str)-1;
        printf("do %d\nc %d\n", x++, c);
        while(c >= 0)
        {
            printf("unget %d\n",c);
            ungetc(str[c--], stdin);
        }
        fgets(rec, sizeof(rec), stdin);
        rec[strcspn(rec, "\n")] = '\0';
        printf("str:%s\nrec:%s\n>>",str, rec);
    } while (1);
}