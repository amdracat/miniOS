#include "MiniOS.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_QUEUE_SIZE 100
#define MAX_QUEUE_COUNT 4

static MessageQueue g_msgQueues[MAX_QUEUE_COUNT];
static pthread_t g_timerThread;
static int g_timerRunning = 0;
static TimerCallback g_timerCallback = NULL;
static uint32_t g_timerInterval = 0;

static MessageQueue *GetQueue(uint32_t queueId) {
    if (queueId >= MAX_QUEUE_COUNT) {
        return NULL;
    }
    return &g_msgQueues[queueId];
}

static void *TimerThreadFunc(void *arg) {
    (void)arg;
    if (g_timerRunning) {
        usleep(g_timerInterval * 1000);
        if (g_timerCallback) {
            g_timerCallback(NULL);
        }
        g_timerRunning = 0;
    }
    return NULL;
}

void OS_Init(void) {
    for (uint32_t i = 0; i < MAX_QUEUE_COUNT; i++) {
        g_msgQueues[i].capacity = MAX_QUEUE_SIZE;
        g_msgQueues[i].queue = (Message *)malloc(sizeof(Message) * g_msgQueues[i].capacity);
        g_msgQueues[i].head = 0;
        g_msgQueues[i].tail = 0;
        g_msgQueues[i].count = 0;
        pthread_mutex_init(&g_msgQueues[i].lock, NULL);
        pthread_cond_init(&g_msgQueues[i].not_empty, NULL);
    }
}

void OS_CreateTask(void (*taskFunc)(void), const char *name) {
    (void)name;
    pthread_t thread;
    pthread_create(&thread, NULL, (void *(*)(void *))taskFunc, NULL);
    pthread_detach(thread);
}

void OS_GetMsg(uint32_t queueId, Message *msg) {
    MessageQueue *queue = GetQueue(queueId);
    if (!queue || !msg) {
        return;
    }

    pthread_mutex_lock(&queue->lock);
    while (queue->count == 0) {
        pthread_cond_wait(&queue->not_empty, &queue->lock);
    }
    *msg = queue->queue[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->count--;
    pthread_mutex_unlock(&queue->lock);
}

void OS_SendMsg(uint32_t queueId, uint32_t msgId, void *data) {
    MessageQueue *queue = GetQueue(queueId);
    if (!queue) {
        return;
    }

    pthread_mutex_lock(&queue->lock);
    if (queue->count < queue->capacity) {
        queue->queue[queue->tail].MsgId = msgId;
        queue->queue[queue->tail].Data = data;
        queue->tail = (queue->tail + 1) % queue->capacity;
        queue->count++;
        pthread_cond_signal(&queue->not_empty);
    }
    pthread_mutex_unlock(&queue->lock);
}

void OS_SetupTimer(TimerCallback callback, uint32_t intervalMs, void* arg) {
    g_timerCallback = callback;
    g_timerInterval = intervalMs;
    g_timerRunning = 1;
    pthread_create(&g_timerThread, NULL, TimerThreadFunc, NULL);
}













