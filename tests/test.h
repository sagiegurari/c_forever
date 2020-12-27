#ifndef __TEST_H__
#define __TEST_H__

#include <stdbool.h>
#include <stddef.h>
#include <sys/stat.h>

void test_run(void (*fn)());
void test_fail();
void assert_true(bool);

void assert_num_equal(size_t, size_t);

#endif

