int initsignal(int signo);
int initread(int fdread, int fdwrite, int signo, char *buf, int bufsize);
int getdone();
int geterror();
int getbytes();