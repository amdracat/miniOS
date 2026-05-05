#ifndef TEST_FRAME_H
#define TEST_FRAME_H

#include <stdio.h>

void init_test_frame(void);
void report_test_result(void);
void test_assert_eq_int(int expected, int actual, const char *file, int line);

#define ASSERT_EQ(expected, actual) \
    test_assert_eq_int((expected), (actual), __FILE__, __LINE__)

#endif /* TEST_FRAME_H */

