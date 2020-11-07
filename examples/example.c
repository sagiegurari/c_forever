#include "forever.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void my_program()
{
  sleep(1);
  exit(0);
}


int callback(const unsigned char started, int stat_loc)
{
  if (stat_loc == 0 || !started)
  {
    return(-1); // no more retries
  }

  return(500); // wait 500 millies before next invocation, 0 for no wait.
}


int main()
{
  // call 'my_program' and when it ends/crashes invoke it again, up
  // to 10 times and wait 250 millies between invocations.
  // counter will hold the amount of times 'my_program' was invoked.
  unsigned int counter = forever_with_options(
    my_program, // function to invoke
    10,         // max amount of retries. 0 for unlimited retries.
    250         // amount of millies to wait between invocations. 0 for no wait.
    );

  printf("Invoked %u time/s.\n", counter);

  // call 'my_program' and when it ends/crashes call the provided callback
  counter = forever_with_callback(my_program, callback);
  printf("Invoked %u time/s.\n", counter);

  // run with unlimited retries and no delay between invocations
  forever(my_program);
}
