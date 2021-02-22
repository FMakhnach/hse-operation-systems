#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

// This program is compiled as prog.out to be used in task8-1.c
int main(int argc, char *argv[]) {
  if (argc == 2) {
    int fd;
    if ((fd = open(argv[1], O_RDONLY)) < 0) {
      printf("task8-2: Cannot open FIFO.\n");
      exit(-1);
    }

    char string[12];
    if (read(fd, string, 12) != 12) {
      printf("task8-2: Cannot read from FIFO.\n");
      exit(-1);
    }

    printf("task8-2: Message that was read from FIFO: %s\n", string);

    if (close(fd) < 0) {
      printf("task8-2: Can't close FIFO.\n");
      exit(-1);
    }
    remove(argv[1]);
  }
  return 0;
}

