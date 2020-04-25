#include "forever.h"
#include "test.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


void fn()
{
  sleep(1);
  exit(0);
}


void test_impl()
{
  clock_t      time    = clock();
  unsigned int counter = forever_with_options(fn, 2, 1000);

  assert_num_equal(counter, 2);
  assert_true(clock() - time >= 4);
}


int main()
{
  test_run(test_impl);
}

