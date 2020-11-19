#include "forever.h"
#include <errno.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

struct ForeverState
{
  void         *context;
  unsigned int max_retries;
  unsigned int interval_in_millies;
  int          (*callback)(void *, const unsigned char, int);
  bool         use_callback;
  unsigned int invocation_counter;
  bool         stop;
};

// private functions
unsigned int _forever(void (*fn)(void *), struct ForeverState);
void _forever_on_exit(struct ForeverState *, const bool, int);
void _forever_msleep(unsigned int);


unsigned int forever(void (*fn)(void *), void *context)
{
  return(forever_with_options(fn, context, 0, 0));
}


unsigned int forever_with_options(void (*fn)(void *), void *context, const unsigned int max_retries, const unsigned int interval_in_millies)
{
  struct ForeverState state =
  {
    context,
    max_retries,
    interval_in_millies,
    NULL,
    false,
    0,
    false,
  };

  return(_forever(fn, state));
}


unsigned int forever_with_callback(void (*fn)(void *), void *context, int (*callback)(void *, const unsigned char, int))
{
  struct ForeverState state =
  {
    context,
    0,
    0,
    callback,
    true,
    0,
    false,
  };

  return(_forever(fn, state));
}


unsigned int _forever(void (*fn)(void *), struct ForeverState state)
{
  state.invocation_counter = 0;
  state.stop               = 0;

  while (!state.stop)
  {
    int pid = fork();
    if (pid < 0)
    {
      // wait and try again later
      _forever_on_exit(&state, false, 0);
    }
    else
    {
      const bool is_parent = pid > 0;

      if (is_parent)
      {
        int stat_loc = 0;
        waitpid(pid, &stat_loc, 0);
        _forever_on_exit(&state, true, stat_loc);
      }
      else
      {
        (*fn)(state.context);
        return(0);
      }
    }
  }

  return(state.invocation_counter);
}


void _forever_on_exit(struct ForeverState *state, const bool started, int stat_loc)
{
  state->invocation_counter++;

  if (state->use_callback)
  {
    const unsigned char started_flag        = started ? 1 : 0;
    const int           interval_in_millies = state->callback(state->context, started_flag, stat_loc);
    if (interval_in_millies < 0)
    {
      state->stop = true;
    }
    else
    {
      state->interval_in_millies = (unsigned int)interval_in_millies;
    }
  }
  else if (state->max_retries > 0 && state->invocation_counter >= state->max_retries)
  {
    state->stop = true;
  }

  if (!state->stop && state->interval_in_millies > 0)
  {
    _forever_msleep(state->interval_in_millies);
  }
}


void _forever_msleep(unsigned int millies)
{
  long            millies_long = (long)millies;

  struct timespec ts;

  ts.tv_sec  = millies_long / 1000;
  ts.tv_nsec = (millies_long % 1000) * 1000000;

  nanosleep(&ts, &ts);
}

