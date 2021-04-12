#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void try_semop(int sem_id, int sem_op) {
  struct sembuf mybuf;
  mybuf.sem_num = 0;
  mybuf.sem_op  = sem_op;
  mybuf.sem_flg = 0;

  if (semop(sem_id, &mybuf, 1) < 0) {
    printf("Can\'t wait for condition\n");
    exit(-1);
  }
}

void P(int sem_id) {
  try_semop(sem_id, -1);
}

void V(int sem_id) {
  try_semop(sem_id, 1);
}

int create_sem() {
  int    semid;
  char   pathname[]="07-3a.c";
  key_t  key;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((semid = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create semaphore set\n");
      exit(-1);
    } else if ((semid = semget(key, 1, 0)) < 0) {
      printf("Can\'t find semaphore\n");
      exit(-1);
    }
  } else {
    // Increase value of new semaphore. 
    V(semid);
  }
  return semid;
}

int main()
{
  int     *array;
  int     shmid;
  int     new = 1;
  char    pathname[] = "07-3a.c";
  key_t   key;
  int     sem_id;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((shmid = shmget(key, 3*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create shared memory\n");
      exit(-1);
    } else {
      if ((shmid = shmget(key, 3*sizeof(int), 0)) < 0) {
        printf("Can\'t find shared memory\n");
        exit(-1);
      }
      new = 0;
    }
  }

  if ((array = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  // Create or get semaphore.
  sem_id = create_sem();

  P(sem_id);
  if (new) {
    array[0] =  0;
    array[1] =  1;
    array[2] =  1;
  } else {
    array[1] += 1;
    array[2] += 1;
  }

  printf
    ("Program 1 was spawn %d times, program 2 - %d times, total - %d times\n",
    array[0], array[1], array[2]);
  V(sem_id);

  if (shmdt(array) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
