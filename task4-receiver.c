#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int value;
int mask;
int sender_pid;

void set_bit_zero(int nsig) {
  mask <<= 1;
  if (mask) {
    // Responding so the next bit can be sent.
    kill(sender_pid, SIGUSR1);
  }
}

void set_bit_one(int nsig) {
  value |= mask;
  mask <<= 1;
  if (mask) {
    // Responding so the next bit can be sent.
    kill(sender_pid, SIGUSR1);
  }
}

int main(void) {
  pid_t my_pid;
  int   i;

  my_pid = getpid();
  printf("My PID = %d\n", my_pid);

  printf("Enter sender PID: ");
  if (scanf("%d", &sender_pid) < 0) {
    printf("Invalid PID\n");
    exit(-1);
  }

  value = 0;
  mask  = 1;
  (void) signal(SIGUSR1, set_bit_zero);
  (void) signal(SIGUSR2, set_bit_one);

  while (mask); // Wait for mask to become 0.
  printf("Received value is %d\n", value);

  return 0;
}
