#include "forever.h"
#include "test.h"
#include <stdlib.h>

struct Context
{
  int counter;
};


void fn(void *context)
{
  ((struct Context *)context)->counter++;
  exit(0);
}


void test_impl()
{
  struct Context *context = malloc(sizeof(struct Context));

  context->counter = 0;

  unsigned int counter = forever_with_options(fn, context, 10, 1);

  assert_num_equal(counter, 10);
  assert_num_equal(context->counter, 0);

  free(context);
}


int main()
{
  test_run(test_impl);
}

