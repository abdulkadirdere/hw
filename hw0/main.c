#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int
main( void ) {
  
  struct rlimit lim;

  // print the max size of the stack
  getrlimit(RLIMIT_STACK, &lim);
  printf("stack size: %zd\n",  lim.rlim_cur);

  // print the max number of threads
  getrlimit(RLIMIT_NPROC, &lim);
  printf("process limit: %zd\n", lim.rlim_cur);

  // print the max number of file descriptors
  getrlimit(RLIMIT_NOFILE, &lim);
  printf("max file descriptors: %zd\n", lim.rlim_cur - 1);
}
