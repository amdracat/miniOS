#include "Test_Module.h"
#include "test_frame.h"
#include "Module.h"
#include <stdio.h>
#include <unistd.h>
#include "../miniOS/OsTestLayer.h"

void test_start(void) {
    init_test_frame();

    Initialize();
    usleep(1000);
    //sleep(1); // Wait for initialization to complete
    ASSERT_EQ(0, GetSpeed());
    ASSERT_EQ(0, GetMode());

    SetSpeed(75);
    usleep(1000);
    //sleep(1); // Wait for speed to be set
    ASSERT_EQ(75, GetSpeed());

    ChangeMode(3);// 同期処理で60ms
    usleep(80000);
    //sleep(1); // Wait for mode to be changed
    ASSERT_EQ(3, GetMode());

    ChangeMode(1);// 非同期で100ms
    usleep(100*1000);
    //sleep(1); // Wait for mode to be changed
    ASSERT_EQ(1, GetMode());


    report_test_result();
}
