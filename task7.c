#define _GNU_SOURCE

#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
  int fd[2];
  if (pipe(fd) < 0) {
    printf("Can\'t open pipe.");
    exit(1);
  }
  long size = (long)fcntl(fd[1], F_GETPIPE_SZ);
  printf("Pipe size is %d bytes.\n", size);
  return 0;
}  

