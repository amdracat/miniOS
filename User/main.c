
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../miniOS/MiniOS.h"
#include "Module.h"
#include "Test_Module.h"

/* Global message buffer */
Message Msg;

/* Global buffer for logging/commands */
char LogBuffer[256];

/* Command handler prototypes */
void CMD1_Handler(void);
void CMD2_Handler(void);
void TimerHandler(void);

/* Command table structure */
typedef struct {
    char cmd[64];
    void (*handler)(void);
} CommandEntry;

/* Command table */
CommandEntry cmdTable[] = {
    {"CMD1", CMD1_Handler},
    {"CMD2", CMD2_Handler},
};

/* Number of commands */
#define NUM_COMMANDS (sizeof(cmdTable) / sizeof(cmdTable[0]))

/* Command handlers */
void CMD1_Handler(void) {
    printf("[CMD1_Handler] Sending MSG_ID_1 to MAIN and SUB queues\n");
    OS_SendMsg(OS_QUEUE_MAIN, MSG_ID_1, NULL);
    OS_SendMsg(OS_QUEUE_SUB, MSG_ID_1, NULL);
}

void CMD2_Handler(void) {
    printf("[CMD2_Handler] Sending MSG_ID_2 to MAIN and SUB queues\n");
    OS_SendMsg(OS_QUEUE_MAIN, MSG_ID_2, NULL);
    OS_SendMsg(OS_QUEUE_SUB, MSG_ID_2, NULL);
}

void TimerHandler(void) {
    printf("[TimerHandler] Sending MSG_ID_TIMER to MAIN and SUB queues\n");
    OS_SendMsg(OS_QUEUE_MAIN, MSG_ID_TIMER, NULL);
    OS_SendMsg(OS_QUEUE_SUB, MSG_ID_TIMER, NULL);
}

/* Main task */
void MainTask(void) {
    printf("[MainTask] Started\n");
    while (1) {
        OS_GetMsg(OS_QUEUE_MAIN, &Msg);
        printf("[MainTask] Received message ID: %d\n", Msg.MsgId);
        
        switch (Msg.MsgId) {
            case MSG_ID_1:
                printf("[MainTask] Handling MSG_ID_1\n");
                break;
            case MSG_ID_2:
                printf("[MainTask] Handling MSG_ID_2\n");
                break;
            case MSG_ID_TIMER:
                printf("[MainTask] Handling MSG_ID_TIMER\n");
                break;
            case MSG_ID_MODULE_EVENT:
                {
                    printf("[MainTask] Handling MSG_ID_MODULE_EVENT\n");
                    ModuleEventData *eventData = (ModuleEventData *)Msg.Data;
                    if (eventData) {
                        MainTask_Event(eventData->event, eventData->value);
                        free(eventData);
                    }
                }
                break;
            default:
                printf("[MainTask] Handling unknown message ID: %d\n", Msg.MsgId);
                break;
        }
    }
}

/* Sub task */
void SubTask(void) {
    printf("[SubTask] Started\n");
    while (1) {
        OS_GetMsg(OS_QUEUE_SUB, &Msg);
        printf("[SubTask] Received message ID: %d\n", Msg.MsgId);
        
        switch (Msg.MsgId) {
            case MSG_ID_1:
                printf("[SubTask] Handling MSG_ID_1\n");
                break;
            case MSG_ID_2:
                printf("[SubTask] Handling MSG_ID_2\n");
                break;
            case MSG_ID_TIMER:
                printf("[SubTask] Handling MSG_ID_TIMER\n");
                break;
            default:
                printf("[SubTask] Handling unknown message ID: %d\n", Msg.MsgId);
                break;
        }
    }
}

/* Log task - reads commands from stdin */
void LogTask(void) {
    printf("[LogTask] Started - Enter commands (CMD1, CMD2) or 'quit' to exit\n");
    while (1) {
        printf("> ");
        fflush(stdout);
        
        /* Read command from stdin */
        if (fgets(LogBuffer, sizeof(LogBuffer), stdin) == NULL) {
            continue;
        }
        
        /* Remove newline character */
        size_t len = strlen(LogBuffer);
        if (len > 0 && LogBuffer[len - 1] == '\n') {
            LogBuffer[len - 1] = '\0';
        }
        
        /* Check for exit command */
        if (strcmp(LogBuffer, "quit") == 0) {
            printf("[LogTask] Exiting...\n");
            exit(0);
        }
        
        if (strcmp(LogBuffer, "test") == 0) {
            test_start();
            continue;
        }
        
        /* Process command */
        int found = 0;
        for (size_t i = 0; i < NUM_COMMANDS; i++) {
            if (strcmp(LogBuffer, cmdTable[i].cmd) == 0) {
                cmdTable[i].handler();
                found = 1;
                break;
            }
        }
        
        if (!found && strlen(LogBuffer) > 0) {
            printf("[LogTask] Unknown command: %s\n", LogBuffer);
        }
    }
}

/* Main function */
int main(void) {
    printf("=== MiniOS Simulator Starting ===\n");
    
    /* Initialize the operating system and other necessary components */
    OS_Init();
    printf("[main] OS Initialized\n");
    
    /* Create tasks */
    OS_CreateTask(MainTask, "Main Task");
    printf("[main] MainTask created\n");
    
    OS_CreateTask(SubTask, "Sub Task");
    printf("[main] SubTask created\n");
    
    OS_CreateTask(LogTask, "Log Task");
    printf("[main] LogTask created\n");
    
    /* Set up a timer to call TimerHandler every 1000 ms */
    OS_SetupTimer(TimerHandler, 1000);
    printf("[main] Timer set up (1000ms interval)\n");
    
    printf("=== MiniOS Simulator Ready ===\n\n");
    
    /* Keep main thread alive */
    while (1) {
        sleep(1);
    }
    
    return 0;
}




