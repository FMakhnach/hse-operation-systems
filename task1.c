#include <signal.h>

int main(void) {
  //
  // Set the process response to the SIGINT and SIGQUIT signals to ignore
  //
  (void)signal(SIGINT, SIG_IGN);
  (void)signal(SIGQUIT, SIG_IGN);
  //
  // From this point, the process will ignore the occurrences of both SIGINT and SIGQUIT signals.
  //
  while(1);
  return 0;
}
