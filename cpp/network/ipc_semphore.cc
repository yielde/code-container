#include <sys/sem.h>
#include <sys/wait.h>
#include <unistd.h>

#include <iostream>

using namespace std;
union semun {
  int val;
  struct semid_ds *buf;
  unsigned short int *array;
  struct seminfo *__buf;
};

void pv(int sem_id, int op) {
  struct sembuf sem_b;
  sem_b.sem_num = 0;
  sem_b.sem_op = op;
  sem_b.sem_flg = SEM_UNDO;
  semop(sem_id, &sem_b, 1);
}

int main(int argc, char *argv[]) {
  int sem_id = semget(IPC_PRIVATE, 1, 0666);
  union semun sem_un;
  sem_un.val = 1;
  semctl(sem_id, 0, SETVAL, sem_un);
  pid_t id = fork();
  if (id < 0) {
    return 1;
  } else if (id == 0) {
    cout << "child try to get binary sem" << endl;
    pv(sem_id, -1);
    cout << "child get the sem and release after 5 seconds" << endl;
    sleep(5);
    pv(sem_id, 1);
    exit(0);
  } else {
    cout << "parent try to get binary sem" << endl;
    pv(sem_id, -1);
    cout << "parent get the sem and release after 5 seconds" << endl;
    sleep(5);
    pv(sem_id, 1);
  }
  waitpid(id, NULL, 0);
  semctl(sem_id, 0, IPC_RMID, sem_un);
  return 0;
}