#include "forever.h"
#include "test.h"
#include <stdlib.h>


void fn(void *context)
{
  assert_true(context == NULL);
  exit(0);
}


void test_impl()
{
  unsigned int counter = forever_with_options(fn, NULL, 10, 1);

  assert_num_equal(counter, 10);
}


int main()
{
  test_run(test_impl);
}

