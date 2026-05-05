#include "Test_Module.h"
#include "test_frame.h"
#include "Module.h"
#include <stdio.h>

void test_start(void) {
    init_test_frame();

    Initialize();
    MainTask_Event(EVENT_INITIALIZE, 0);
    ASSERT_EQ(0, GetSpeed());
    ASSERT_EQ(0, GetMode());

    SetSpeed(75);
    MainTask_Event(EVENT_SET_SPEED, 75);
    ASSERT_EQ(75, GetSpeed());

    ChangeMode(3);
    MainTask_Event(EVENT_CHANGE_MODE, 3);
    ASSERT_EQ(3, GetMode());

    report_test_result();
}
