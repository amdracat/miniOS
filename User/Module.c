#include "Module.h"
#include "../miniOS/MiniOS.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
static int g_speed = 0;
static int g_mode = 0;
static int g_initialized = 0;

static void send_module_event(MainTaskEvent event, int value) {
    ModuleEventData *data = (ModuleEventData *)malloc(sizeof(ModuleEventData));
    if (!data) {
        printf("[send_module_event] Failed to allocate memory for ModuleEventData\n");
        return;
    }
    data->event = event;
    data->value = value;
    printf("[send_module_event] Sending event: %d value=%d\n", event, value);
    OS_SendMsg(OS_QUEUE_MAIN, MSG_ID_MODULE_EVENT, data);
}

void Initialize(void) {
    send_module_event(EVENT_INITIALIZE, 0);
}

void SetSpeed(int speed) {
    send_module_event(EVENT_SET_SPEED, speed);
}

int GetSpeed(void) {
    return g_speed;
}

void ChangeMode(int mode) {
    send_module_event(EVENT_CHANGE_MODE, mode);
}

int GetMode(void) {
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
            printf("[MainTask_Event] EVENT_SET_SPEED: %d value=%d\n",g_initialized, value);
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
