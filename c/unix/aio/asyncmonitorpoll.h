#define NUMOPS 8
int initaio(int fd, int handle);
int readcheck(int handle, char **bufp);
int readstart(int handle);
void reinit(int handle);