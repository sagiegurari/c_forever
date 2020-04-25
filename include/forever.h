#ifndef __FOREVER_H__
#define __FOREVER_H__

unsigned int forever(void (*fn)());
unsigned int forever_with_options(void (*fn)(), const unsigned int /* max retries */, const unsigned int /* interval between invocations in milliseconds */);
unsigned int forever_with_callback(void (*fn)(), int (*callback)(const unsigned char, int));

#endif

