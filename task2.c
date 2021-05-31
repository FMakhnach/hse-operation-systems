#include <signal.h>
#include <stdio.h>

void my_handler(int nsig) {
  if (nsig == SIGINT) {
    printf("Receive signal %d, CTRL-C pressed\n", nsig);
  } else if (nsig == SIGQUIT) {
    printf("Receive signal %d, CTRL-4 pressed. Different message!\n", nsig);
  } else {
    printf("Signal %d is not supported by handler\n", nsig);
  }
}

int main(void) {
  //
  // Set the process response to the SIGINT and SIGQUIT signals.
  //
  (void)signal(SIGINT, my_handler);
  (void)signal(SIGQUIT, my_handler);
  //
  // From this point, the process will print SIGINT or SIGQUIT messages.
  //
  while(1);
  return 0;
}
