#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main()
{
  char    *filetext;
  int     shmid;
  char    pathname[] = "task1a.c";
  key_t   key;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can't generate key\n");
    exit(-1);
  }
  // Getting size of the file.
  struct stat st;
  stat(pathname, &st);
  int size = st.st_size;

  if ((shmid = shmget(key, size, 0666|IPC_CREAT|IPC_EXCL)) < 0) {
    //
    // In case of an error, try to determine: whether it arose due to the fact
    // that the shared memory segment already exists or for some other reason.
    //
    if (errno != EEXIST) {
      printf("Can't create shared memory\n");
      exit(-1);
    } else {
      //
      // If because the shared memory already exists,
      // try to get its IPC descriptor and,
      // if successful, reset the Initialization flag.
      //
      if ((shmid = shmget(key, size, 0)) < 0) {
        printf("Can't find shared memory\n");
        exit(-1);
      }
    }
  }

  // Try to map shared memory to the address space of the current process.
  if ((filetext = (char *)shmat(shmid, NULL, 0)) == (char *)(-1)) {
    printf("Can't attach shared memory\n");
    exit(-1);
  }

  FILE *fp = fopen(pathname, "r");
  fread(filetext, 1, size, fp);

  printf ("Successfully wrote file text to shared memory.\n");

  if (shmdt(filetext) < 0) {
    printf("Can't detach shared memory\n");
    exit(-1);
  }

  return 0;
}
