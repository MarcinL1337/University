#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main () {
  pid_t child_pid = fork ();
  if (child_pid > 0) { // parent
    sleep (100);
  }
  else { // child
    exit (0);
  }
  return 0;
}
