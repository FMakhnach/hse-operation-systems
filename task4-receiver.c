#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

int value;
int mask;

void set_bit_zero(int nsig) {
  mask <<= 1;
}

void set_bit_one(int nsig) {
  value |= mask;
  mask <<= 1;
}

int main(void) {
  pid_t my_pid, sender_pid;
  int   i;

  value = 0;
  mask  = 1;
  (void) signal(SIGUSR1, set_bit_zero);
  (void) signal(SIGUSR2, set_bit_one);

  my_pid = getpid();
  printf("My PID = %d\n", my_pid);

  printf("Enter sender PID: ");
  scanf("%d", &sender_pid);

  while (1) {
    if(mask == 0) {
      printf("Received value is %d\n", value);
      break;
    }
  }
  return 0;
}
