#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define BUFFER_LENGTH 256

int main()
{
    char *str = malloc(BUFFER_LENGTH), *rec = malloc(64);
    fputs(">>", stdout);
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON);
    char c;
    do
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        c = getchar();
        printf("%d\n", c);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    } while (c!='q');
}