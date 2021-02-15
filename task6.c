#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd[2], fd2[2], result;

  size_t size;
  char  resstring[14];

  if (pipe(fd) < 0) {
    printf("Can\'t open pipe 1\n");
    exit(-1);
  }

  if (pipe(fd2) < 0) {
    printf("Can\'t open pipe 2\n");
    exit(-1);
  }

  result = fork();

  if (result < 0) {
    printf("Can\'t fork child\n");
    exit(-1);
  } else if (result > 0) {

   /* Parent process */

    if (close(fd[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe\n"); exit(-1);
    }

    size = write(fd[1], "Hello, world!", 14);

    if (size != 14) {
      printf("Can\'t write all string to pipe\n");
      exit(-1);
    }

    if (close(fd[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe\n"); exit(-1);
    }

    if (close(fd2[1]) < 0) {
      printf("parent: Can\'t close writing side of pipe 2\n"); exit(-1);
    }

    size = read(fd2[0], resstring, 12);

    if (size < 0) {
      printf("Can\'t read string from pipe 2\n");
      exit(-1);
    }

    if (close(fd2[0]) < 0) {
      printf("parent: Can\'t close reading side of pipe 2\n"); exit(-1);
    }

    printf("Parent exit, resstring:%s\n", resstring);

  } else {

    /* Child process */

    if (close(fd2[0]) < 0) {
      printf("child: Can\'t close reading side of pipe 2\n"); exit(-1);
    }

    size = write(fd2[1], "Bye, world!", 12);

    if (size != 12) {
      printf("Can\'t write all string to pipe 2\n");
      exit(-1);
    }

    if (close(fd2[1]) < 0) {
      printf("child: Can\'t close writing side of pipe 2\n"); exit(-1);
    }

    if (close(fd[1]) < 0) {
      printf("child: Can\'t close writing side of pipe\n"); exit(-1);
    }

    size = read(fd[0], resstring, 14);

    if (size < 0) {
      printf("Can\'t read string from pipe\n"); exit(-1);
    }

    printf("Child exit, resstring:%s\n", resstring);

    if (close(fd[0]) < 0) {
      printf("child: Can\'t close reading side of pipe\n"); exit(-1);
    }
  }

  return 0;
}
