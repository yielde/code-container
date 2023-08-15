#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define PERMS (S_IRUSR | S_IWUSR)
#define TEN_MILLION 10000000L

int initelement(int semid, int semnum, int semvalue) {
  union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
  } arg;
  arg.val = semvalue;
  return semctl(semid, semnum, SETVAL, arg);
}

int initsemset(key_t mykey, int value, sig_atomic_t *readyp) {
  int semid;
  struct timespec sleeptime;
  sleeptime.tv_sec = 0;
  sleeptime.tv_nsec = TEN_MILLION;
  semid = semget(mykey, 1, PERMS | IPC_CREAT | IPC_EXCL);
  if ((semid == -1) && (errno != EEXIST)) return -1;
  if (semid >= 0) {
    if (initelement(semid, 0, value) == -1) return -1;
    *readyp = 1;
    return semid;
  }
  if ((semid = semget(mykey, 1, PERMS)) == -1) return -1;
  while (*readyp == 0) {
    nanosleep(&sleeptime, NULL);
  }
  return semid;
}

void setsembuf(struct sembuf *s, int num, int op, int flg) {
  s->sem_num = (short)num;
  s->sem_op = (short)op;
  s->sem_flg = (short)flg;
  return;
}

typedef struct {
  int count;
  double sum;
  sig_atomic_t ready;
} shared_sum_t;

static int semid;
static struct sembuf semlock;
static struct sembuf semunlock;

static shared_sum_t *sharedsum;

int initshared(int key) {
  int shid;
  setsembuf(&semlock, 0, -1, 0);
  setsembuf(&semunlock, 0, 1, 0);

  shid = shmget(key, sizeof(shared_sum_t), PERMS | IPC_CREAT | IPC_EXCL);
  if ((shid == -1) && (errno != EEXIST)) return -1;
  if (shid == -1) {
    if (((shid = shmget(key, sizeof(shared_sum_t), PERMS)) == -1) ||
        ((sharedsum = (shared_sum_t *)shmat(shid, NULL, 0)) == (void *)-1))
      return -1;
  } else {
    sharedsum = (shared_sum_t *)shmat(shid, NULL, 0);
    if (sharedsum == (void *)-1) return -1;
    sharedsum->count = 0;
    sharedsum->sum = 0.0;
  }
  semid = initsemset(key, 1, &sharedsum->ready);
  if (semid == -1) return -1;
  return 0;
}

int add(double x) {
  if (semop(semid, &semlock, 1) == -1) return -1;
  sharedsum->sum += x;
  sharedsum->count++;
  if (semop(semid, &semunlock, 1) == -1) return -1;
  return 0;
}

int getcountandsum(int *countp, double *sum) {
  if (semop(semid, &semlock, 1) == -1) return -1;
  *countp = sharedsum->count;
  *sum = sharedsum->sum;
  if (semop(semid, &semunlock, 1) == -1) return -1;
  return 0;
}

static void showit(int signo) {
  int count;
  double sum;
  if (getcountandsum(&count, &sum) == -1)
    printf("Failed to get count and sum\n");
  else
    printf("Sum is %f, count is %d\n", sum, count);
}

int main(int argc, char *argv[]) {
  struct sigaction act;
  int key;
  sigset_t mask, oldmask;

  if (argc != 2) {
    fprintf(stderr, "Usage: %s key\n", argv[0]);
    return 1;
  }
  key = atoi(argv[1]);
  if (initshared(key) == -1) {
    perror("Failed to initialize shared memory");
    return 1;
  }

  if ((sigfillset(&mask) == -1) ||
      (sigprocmask(SIG_SETMASK, &mask, &oldmask) == -1)) {
    perror("Failed to block signals to set up handlers");
    return 1;
  }

  printf("This is process %ld waiting for SIGUSR1 (%d)\n", (long)getpid(),
         SIGUSR1);
  act.sa_handler = showit;
  act.sa_flags = 0;
  if ((sigemptyset(&act.sa_mask) == -1) ||
      (sigaction(SIGUSR1, &act, NULL) == -1)) {
    perror("Failed to set up signal handler");
    return 1;
  }

  if (sigprocmask(SIG_SETMASK, &oldmask, NULL) == -1) {
    perror("Failed to unblock signals");
    return 1;
  }

  for (;;) {
    pause();
  }
}