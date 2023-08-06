int geterror(int *error);
int seterror(int error);

int add(double x);
int getsum(double *sump);
int getcountandsum(int *countp, double *sump);

int getdone(int *flag);
int setdone();

int randsafe(double *randp);
void *computethread(void *arg1);