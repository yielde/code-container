#include <arpa/inet.h>

void addr2name(struct in_addr addr, char *name, int namelen);
int name2addr(const char *name, in_addr_t *addrp);