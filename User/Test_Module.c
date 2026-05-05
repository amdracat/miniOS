#include "Test_Module.h"
#include "test_frame.h"
#include "Module.h"
#include <stdio.h>
#include <unistd.h>
#include "../miniOS/OsTestLayer.h"

void test_start(void) {
    init_test_frame();

    Initialize();
    OsTestLayer_Sim_RunAll(); // Run all pending events after initialization
    //usleep(1000);
    ASSERT_EQ(0, GetSpeed());
    ASSERT_EQ(0, GetMode());

    SetSpeed(75);
    OsTestLayer_Sim_RunAll();
    //usleep(1000);
    ASSERT_EQ(75, GetSpeed());

    ChangeMode(3);// 同期処理で60ms
    OsTestLayer_Sim_RunAll();
    usleep(80000);
    ASSERT_EQ(3, GetMode());

    ChangeMode(1);// 非同期で100ms
    OsTestLayer_Sim_RunAll();
    OsTestLayer_Sim_AdvanceTime(100); // Advance time to trigger async mode change
    OsTestLayer_Sim_RunAll();
    
    //usleep(100*1000);
    ASSERT_EQ(1, GetMode());

    ChangeMode(2);// 非同期で100ms+100ms
    //usleep(100*1000);
    //usleep(100*1000);

    OsTestLayer_Sim_RunAll();
    OsTestLayer_Sim_AdvanceTime(100); // Advance time to trigger async mode change
    OsTestLayer_Sim_RunAll();
    OsTestLayer_Sim_AdvanceTime(100); // Advance time to trigger async mode change
    OsTestLayer_Sim_RunAll();

    ASSERT_EQ(2, GetMode());


    report_test_result();
}
