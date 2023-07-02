#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int makeargv(const char *s, const char *delimiters, char ***argvp)
{
    int error;
    int i;
    int numtokens;
    const char *snew;
    char *t;

    if ((s == NULL) || (delimiters == NULL) || (argvp == NULL))
    {
        errno = EINVAL;
        return -1;
    }

    *argvp = NULL;
    snew = s + strspn(s, delimiters); // 去除argv[1]最前面的空格

    if ((t = malloc(strlen(snew) + 1)) == NULL)
    {
        return -1;
    }
    strcpy(t, snew);
    numtokens = 0;
    if (strtok(t, delimiters) != NULL)
        for (numtokens = 1; strtok(NULL, delimiters) != NULL; numtokens++)
            printf("aaa : %s\n", t);

    if ((*argvp = malloc((numtokens + 1) * sizeof(char *))) == NULL)
    {
        error = errno;
        free(t);
        errno = error;
        return -1;
    }

    if (numtokens == 0)
        free(t);
    else
    {
        strcpy(t, snew);
        **argvp = strtok(t, delimiters);
        for (i = 1; i < numtokens; i++)
        {
            *((*argvp) + i) = strtok(NULL, delimiters);
        }
        *((*argvp) + numtokens) = NULL;
        return numtokens;
    }
}

int main(int argc, char *argv[])
{
    char delim[] = " \t";
    int i;
    char **myargv;
    int numtokens;
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s string\n", argv[0]);
        return 1;
    }

    if ((numtokens = makeargv(argv[1], delim, &myargv)) == -1)
    {
        fprintf(stderr, "Failed to construct an argument array for %s\n", argv[1]);
        return 1;
    }
    printf("The argument array contains: \n");
    for (i = 0; i < numtokens; i++)
    {
        printf("%d:%s\n", i, myargv[i]);
    }
    return 0;
}