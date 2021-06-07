#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
  pid_t my_pid, receiver_pid;
  int   input, mask;
  int   i;

  my_pid = getpid();
  printf("My PID = %d\n", my_pid);

  printf("Enter receiver PID: ");
  scanf("%d", &receiver_pid);

  printf("Enter an integer value to send: ");
  scanf("%d", &input);

  mask = 1;
  while (mask != 0) {
    if (input & mask) {
      kill(receiver_pid, SIGUSR2);
    } else {
      kill(receiver_pid, SIGUSR1);
    }
    mask <<= 1;
    for (i = 0; i < 30000000; ++i);
  }

  return 0;
}
