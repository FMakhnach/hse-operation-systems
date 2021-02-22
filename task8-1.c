#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
  int     fd;
  char    name[] = "bbb.fifo";

  (void)umask(0);

  if (mknod(name, S_IFIFO | 0666, 0) < 0) {
    printf("task8-1: Can't create FIFO\n");
    exit(-1);
  }

  const char *prog = "./prog.out";

  if ((fd = open(name, O_RDWR | O_NDELAY)) < 0) {
    printf("task8-1: Can\'t open FIFO for reading and writing.\n");
    exit(1);
  }

  char *value = "Useful info";
  if (write(fd, value, 12) != 12) {
    printf("task8-1: Failed to write to FIFO.\n");
    exit(1);
  }

  printf("task8-1: Written to FIFO: %s\n", value);
  execlp(prog, prog, name, NULL);

  printf("Failed to start program prog.out.\n");
  if (close(fd) < 0) {
    printf("Can't close FIFO.\n");
    exit(1);
  }

  return 0;
}

