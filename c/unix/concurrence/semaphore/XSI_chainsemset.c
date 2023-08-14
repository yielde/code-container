#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../../restart.h"

#define BUFSIZE 1024
#define PERMS (S_IRUSR | S_IWUSR)

void setsembuf(struct sembuf *s, int num, int op, int flg) {
  s->sem_flg = (short)flg;
  s->sem_num = (short)num;
  s->sem_op = (short)op;
  return;
}

int initelement(int semid, int semnum, int semvalue) {
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  } arg;

  arg.val = semvalue;
  return semctl(semid, semnum, SETVAL, arg);
}

int removesem(int semid) { return semctl(semid, 0, IPC_RMID); }

int r_semop(int semid, struct sembuf *sops, int nsops) {
  while (semop(semid, sops, nsops) == -1) {
    if (errno != EINTR) {
      return -1;
    }
  }
  return 0;
}

void printerror(char *msg, int error) {
  fprintf(stderr, "[%ld] %s: %s\n", (long)getpid(), msg, strerror(error));
}

int main(int argc, char *argv[]) {
  char buffer[MAX_CANON];
  char *c;
  pid_t childpid;
  int delay;
  int error;
  int semid;
  int n, i;
  struct sembuf semsignal[1];
  struct sembuf semwait[1];

  if ((argc != 3) || ((n = atoi(argv[1])) <= 0) ||
      ((delay = atoi(argv[2])) < 0)) {
    fprintf(stderr, "Usage: %s processes delay\n", argv[0]);
    return 1;
  }
  if ((semid = semget(IPC_PRIVATE, 1, PERMS)) == -1) {
    perror("Failed to create a private semaphore");
    return 1;
  }
  setsembuf(semwait, 0, -1, 0);
  setsembuf(semsignal, 0, 1, 0);

  if (initelement(semid, 0, 1) == -1) {
    perror("Failed to initialize semaphore element to 1");
    if (removesem(semid) == -1) perror("Failed to remove failed semaphore");
    return 1;
  }

  for (i = 1; i < n; i++)
    if (childpid = fork()) break;
  snprintf(buffer, BUFSIZE, "i:%d PID:%ld PPID:%ld CPID:%ld\n", i,
           (long)getpid(), (long)getppid(), (long)childpid);
  c = buffer;
  if (((error = r_semop(semid, semwait, 1)) == -1) && (i > 1)) {
    printerror("Child failed to lock semid", error);
    return 1;
  } else if (!error) {
    while (*c != '\0') {
      fputc(*c, stderr);
      c++;
      for (int j = 0; j < delay; j++)
        ;
    }
    if ((error = r_semop(semid, semsignal, 1)) == -1)
      printerror("Failed to unlock semid", error);
  }

  if ((r_wait(NULL) == -1) && (errno != ECHILD))
    printerror("Failed to unlock semid", error);

  if ((i == 1) && ((error = removesem(semid)) == -1)) {
    printerror("Failed to clean up", error);
    return 1;
  }
  return 0;
}
