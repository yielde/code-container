`#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char str[] = {"wssessaddssffsfhshh"};
    const char charset = 'ss';
    char *t;
    int count;
    t = strtok(str, charset);
    while (t != NULL)
    {
        printf("%s\n", t);
        t = strtok(NULL, charset);
    }
    return 0;
}
