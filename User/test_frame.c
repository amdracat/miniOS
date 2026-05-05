#include "test_frame.h"
#include <time.h>

static int g_test_total = 0;
static int g_test_failures = 0;
static clock_t g_test_start_time;

void init_test_frame(void) {
    g_test_total = 0;
    g_test_failures = 0;
    g_test_start_time = clock();
}

void test_assert_eq_int(int expected, int actual, const char *file, int line) {
    g_test_total++;
    if (expected != actual) {
        g_test_failures++;
        printf("ASSERT_EQ failed: %s:%d expected=%d actual=%d\n", file, line, expected, actual);
    }
}

void report_test_result(void) {
    double elapsed = (double)(clock() - g_test_start_time) / CLOCKS_PER_SEC;
    printf("\n[Test Report] total=%d failures=%d elapsed=%.3f sec\n", g_test_total, g_test_failures, elapsed);
}
