#include "forever.h"
#include "test.h"
#include <stdlib.h>


void fn()
{
  exit(0);
}


void test_impl()
{
  unsigned int counter = forever_with_options(fn, 1, 0);

  assert_num_equal(counter, 1);
}


int main()
{
  test_run(test_impl);
}

