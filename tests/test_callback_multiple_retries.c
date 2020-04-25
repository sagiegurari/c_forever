#include "forever.h"
#include "test.h"
#include <stdlib.h>
#include <time.h>

unsigned short global_counter = 0;


int callback(const unsigned char started, int stat_loc)
{
  assert_true(started);
  assert_num_equal(stat_loc, 0);

  global_counter++;

  if (global_counter < 4)
  {
    return(500);
  }

  return(-1);
}


void fn()
{
  exit(0);
}


void test_impl()
{
  clock_t      time    = clock();
  unsigned int counter = forever_with_callback(fn, callback);

  assert_num_equal(counter, 4);
  assert_true(clock() - time >= 2);
}


int main()
{
  test_run(test_impl);
}

