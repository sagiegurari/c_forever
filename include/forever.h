#ifndef __FOREVER_H__
#define __FOREVER_H__

unsigned int forever(void (*fn)(void *), void *);
unsigned int forever_with_options(void (*fn)(void *), void *, const unsigned int /* max retries */, const unsigned int /* interval between invocations in milliseconds */);
unsigned int forever_with_callback(void (*fn)(void *), void *, int (*callback)(void *, const unsigned char, int));

#endif

