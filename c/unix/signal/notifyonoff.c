#include <errno.h>
#include <signal.h>
#include <stdio.h>

static volatile sig_atomic_t notifyflag = 1;
static int signal1 = 0;
static int signal2 = 0;

static void turnon(int s) { notifyflag = 1; }
static void turnoff(int s) { notifyflag = 0; }

int initnotify(int signo1, int signo2) {
  signal1 = signo1;
  signal2 = signo2;
  struct sigaction newact;
  newact.sa_flags = 0;
  newact.sa_handler = turnon;

  if ((sigemptyset(&newact.sa_mask) == -1) ||
      (sigaddset(&newact.sa_mask, signo1) == -1) ||
      (sigaddset(&newact.sa_mask, signo2) == -1) ||
      (sigaction(signo1, &newact, NULL) == -1))
    return -1;
  newact.sa_handler = turnoff;
  if (sigaction(signo2, &newact, NULL) == -1) return -1;
  return 0;
}

int waitnotifyon() {
  sigset_t maskblocked, maskunblocked, maskold;
  if ((sigprocmask(SIG_SETMASK, NULL, &maskblocked) == -1) ||
      (sigprocmask(SIG_SETMASK, NULL, &maskunblocked) == -1) ||
      (sigaddset(&maskblocked, signal1) == -1) ||
      (sigaddset(&maskblocked, signal2) == -1) ||
      (sigdelset(&maskunblocked, signal1) == -1) ||
      (sigdelset(&maskunblocked, signal2) == -1) ||
      (sigprocmask(SIG_BLOCK, &maskblocked, &maskold) == -1))
    return -1;
  while (notifyflag == 0) sigsuspend(&maskunblocked);
  if (sigprocmask(SIG_SETMASK, &maskold, NULL) == -1) return -1;
  return 0;
}