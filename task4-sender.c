#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int cant_send;

void allow_send(int nsig) {
  cant_send = 0;
}

int main(void) {
  pid_t my_pid, receiver_pid;
  int   input, mask;
  int   i;

  my_pid = getpid();
  printf("My PID = %d\n", my_pid);

  printf("Enter receiver PID: ");
  if (scanf("%d", &receiver_pid) < 0) {
    printf("Invalid PID\n");
    exit(-1);
  }

  printf("Enter an integer value to send: ");
  if (scanf("%d", &input) < 0) {
    printf("Invalid integer value\n");
    exit(-1);
  }

  (void) signal(SIGUSR1, allow_send);

  mask = 1;
  while (mask != 0) {
    while(cant_send); // Wait for client to respond.
    cant_send = 1;

    if (input & mask) {
      kill(receiver_pid, SIGUSR2);
    } else {
      kill(receiver_pid, SIGUSR1);
    }
    mask <<= 1;
  }

  return 0;
}
