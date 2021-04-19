#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/* 
 * Program description.
 * 
 * There are 2 processes, 1 common pipe and 1 IPC semaphore.
 * 1st and 2nd processes (parent and child) are sequentially performing 2 actions N times (in 'for' loop):
 *  1) read message from the pipe (exception: first iteration of parent loop);
 *  2) write message to the pipe;
 * Syncronization is performed the following way:
 *  0) semaphore (sem) holds value 2 at the start;
 *  1) parent performs operation D(sem, 1) both on enter and exit for loop body 
 *     (waits for sem>0 to start, sem=1 when parent works, sem=0 when parent finishes);
 *  2) child performs operation Z(sem) on enter and A(sem, 2) on exit loop body 
 *     (waits for sem=0 to start, sem=0 when child works, sem=2 on exit)
 * This way parent and child communicate sequentially through pipe for N times.
 */

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

void A(int sem_id, int value) {
  try_semop(sem_id, value);
}

void D(int sem_id, int value) {
  try_semop(sem_id, -value);
}

void Z(int sem_id) {
  try_semop(sem_id, 0);
}

int create_or_get_sem() {
  int    sem_id;
  char   pathname[]="05-3.c";
  key_t  key;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((sem_id = semget(key, 1, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    if (errno != EEXIST) {
      printf("Can\'t create semaphore set\n");
      exit(-1);
    } else if ((sem_id = semget(key, 1, 0)) < 0) {
      printf("Can\'t find semaphore\n");
      exit(-1);
    }
  } else {
    // Increase value of new semaphore by 2.
    A(sem_id, 2);
  }
  return sem_id;
} 

int main(int argc, char** argv)
{
  int     fd[2], result;
  size_t  size;
  
  char    message[15];
  int     n = 0;

  if (argc < 2) {
    printf("You must provide N as an argument.\n");
    exit(-1);
  }

  int parse_status = sscanf(argv[1], "%d", &n);
  if (parse_status < 0) {
    printf("Cannot parse N.\n");
    exit(-1);
  }

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe\n");
    exit(-1);
  }

  result = fork();

  // Create or get semaphore.
  int sem_id = create_or_get_sem();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

    /* Parent process */

    for (int i = 1; i <= n; ++i) {
      D(sem_id, 1);

      if (i != 1) {
        // Read message from child.
        size = read(fd[0], message, 15);
        if (size < 0) {
          printf("Can\'t read string from pipe\n");
          exit(-1);
        }
        printf("%d. Parent read message:%s\n", i, message);
      }

      // Write message to child.
      size = write(fd[1], "Hello, child!", 14);
      if (size != 14) {
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      D(sem_id, 1);
    }

    printf("Parent exit\n");
  } else {
    /* Child process */

    for (int i = 1; i <= n; ++i) {
      Z(sem_id);

      // Read message from parent.
      size = read(fd[0], message, 14);
      if (size < 0) {
        printf("Can\'t read string from pipe\n");
        exit(-1);
      }
      printf("%d. Child read message:%s\n", i, message);

      // Write message to parent.
      size = write(fd[1], "Hello, parent!", 15);
      if (size != 15) {
        printf("Can\'t write all string to pipe\n");
        exit(-1);
      }

      A(sem_id, 2);
    }

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }
    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n"); exit(-1);
    }
    printf("Child exit\n");
  }

  return 0;
}
