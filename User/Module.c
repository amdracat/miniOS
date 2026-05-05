#include "Module.h"
#include "../miniOS/MiniOS.h"
#include <stdlib.h>
#include <unistd.h>

static int g_speed = 0;
static int g_mode = 0;
static int g_initialized = 0;

static void send_module_event(MainTaskEvent event, int value) {
    ModuleEventData *data = (ModuleEventData *)malloc(sizeof(ModuleEventData));
    if (!data) {
        return;
    }
    data->event = event;
    data->value = value;
    OS_SendMsg(MSG_ID_MODULE_EVENT, data);
}

void Initialize(void) {
    send_module_event(EVENT_INITIALIZE, 0);
}

void SetSpeed(int speed) {
    send_module_event(EVENT_SET_SPEED, speed);
}

int GetSpeed(void) {
    send_module_event(EVENT_GET_SPEED, 0);
    return g_speed;
}

void ChangeMode(int mode) {
    send_module_event(EVENT_CHANGE_MODE, mode);
}

int GetMode(void) {
    send_module_event(EVENT_GET_MODE, 0);
    return g_mode;
}

void MainTask_Event(MainTaskEvent event, int value) {
    switch (event) {
        case EVENT_INITIALIZE:
            g_initialized = 1;
            g_speed = 0;
            g_mode = 0;
            break;
        case EVENT_SET_SPEED:
            if (g_initialized) {
                g_speed = value;
            }
            break;
        case EVENT_GET_SPEED:
            break;
        case EVENT_CHANGE_MODE:
            if (g_initialized) {
                usleep(30000);
                g_mode = value;
            }
            break;
        case EVENT_GET_MODE:
            break;
        default:
            break;
    }
}
