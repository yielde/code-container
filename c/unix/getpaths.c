#include <stdlib.h>
#include <stdio.h>
#define PATH_DELIMITERS ":"

int makeargv(const char *s, const char *delimiters, char ***argvp);

char **getpaths(void) {
  char **myargv;
  char *path;

  path = getenv("PATH");
  if (makeargv(path, PATH_DELIMITERS, &myargv) == -1)
    return NULL;
  else
    return myargv;
}

int main(void) {
    char **result = getpaths();
    for (int i; result[i]!=NULL;i++){
        printf("%s", result[i]);
    }
}