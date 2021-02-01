#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t pid = getpid();
  pid_t ppid = getppid();

  printf("Current process PID=%d, PPID=%d.\n", pid, ppid);
  
  // Each consecutive execution increments PID value, PPID value remains the same.
  // OS gives every new process a PID equal to the last process PID plus 1. 

  return 0;
}

