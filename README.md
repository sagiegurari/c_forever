# forever

[![CI](https://github.com/sagiegurari/c_forever/workflows/CI/badge.svg?branch=master)](https://github.com/sagiegurari/c_forever/actions)
[![Release](https://img.shields.io/github/v/release/sagiegurari/c_forever)](https://github.com/sagiegurari/c_forever/releases)
[![license](https://img.shields.io/github/license/sagiegurari/c_forever)](https://github.com/sagiegurari/c_forever/blob/master/LICENSE)

> Ensure the program runs continuously.

* [Overview](#overview)
* [Usage](#usage)
* [Contributing](.github/CONTRIBUTING.md)
* [Release History](CHANGELOG.md)
* [License](#license)

<a name="overview"></a>
## Overview
This library provides few utility functions which enable you to invoke a function and ensure it continues to execute.<br>
In case the function exists due to a crash or simply because it finished, the forever library will reinvoke it.<br>
The forever function provides control over the amount of retries and time to wait between intervals via parameters or via callback for a more
fine grained control.<br>
The provided function is invoked as a forked process, so any crash is caught and when restarts, it will be started as a new process with a new state.

<a name="usage"></a>
## Usage

<!-- example source start -->
```c
#include "forever.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void my_program(void *context)
{
  if (context != NULL)
  {
    // do something with the context
  }

  sleep(1);
  exit(0);
}


int callback(void *context, const unsigned char started, int stat_loc)
{
  if (context != NULL)
  {
    // do something with the context
  }

  if (stat_loc == 0 || !started)
  {
    return(-1); // no more retries
  }

  return(500); // wait 500 millies before next invocation, 0 for no wait.
}


int main()
{
  void *context = NULL;

  // call 'my_program' and when it ends/crashes invoke it again, up
  // to 10 times and wait 250 millies between invocations.
  // counter will hold the amount of times 'my_program' was invoked.
  unsigned int counter = forever_with_options(
    my_program, // function to invoke
    context,    // context that is passed to the function on every invocation
    10,         // max amount of retries. 0 for unlimited retries.
    250         // amount of millies to wait between invocations. 0 for no wait.
    );

  printf("Invoked %u time/s.\n", counter);

  // call 'my_program' and when it ends/crashes call the provided callback
  counter = forever_with_callback(my_program, context, callback);
  printf("Invoked %u time/s.\n", counter);

  // run with unlimited retries and no delay between invocations
  forever(my_program, context);
}
```
<!-- example source end -->

## Contributing
See [contributing guide](.github/CONTRIBUTING.md)

<a name="history"></a>
## Release History

See [Changelog](CHANGELOG.md)

<a name="license"></a>
## License
Developed by Sagie Gur-Ari and licensed under the Apache 2 open source license.
