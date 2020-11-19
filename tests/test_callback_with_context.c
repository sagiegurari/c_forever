#include "forever.h"
#include "test.h"
#include <stdlib.h>
#include <time.h>

struct Context
{
  int counter;
};


int callback(void *context, const unsigned char started, int stat_loc)
{
  ((struct Context *)context)->counter++;
  assert_true(started);
  assert_num_equal(stat_loc, 0);

  if (((struct Context *)context)->counter < 4)
  {
    return(500);
  }

  return(-1);
}


void fn(void *context)
{
  assert_true(context != NULL);
  exit(0);
}


void test_impl()
{
  struct Context *context = malloc(sizeof(struct Context));

  context->counter = 0;

  clock_t      time    = clock();
  unsigned int counter = forever_with_callback(fn, context, callback);
  free(context);

  assert_num_equal(counter, 4);
  assert_true(clock() - time >= 2);
}


int main()
{
  test_run(test_impl);
}

