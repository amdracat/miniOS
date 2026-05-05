#ifndef MINIOS_H
#define MINIOS_H

#include <pthread.h>
#include <stdint.h>

#define MSG_ID_1      1
#define MSG_ID_2      2
#define MSG_ID_TIMER  3
#define MSG_ID_MODULE_EVENT 100

#define OS_QUEUE_MAIN 0
#define OS_QUEUE_SUB  1
#define OS_QUEUE_LOG  2

typedef struct {
    uint32_t MsgId;
    void *Data;
} Message;

typedef struct {
    Message *queue;
    int head;
    int tail;
    int count;
    int capacity;
    pthread_mutex_t lock;
    pthread_cond_t not_empty;
} MessageQueue;

typedef void (*TimerCallback)(void);

void OS_Init(void);
void OS_CreateTask(void (*taskFunc)(void), const char *name);
void OS_GetMsg(uint32_t queueId, Message *msg);
void OS_SendMsg(uint32_t queueId, uint32_t msgId, void *data);
void OS_SetupTimer(TimerCallback callback, uint32_t intervalMs);

#endif /* MINIOS_H */
