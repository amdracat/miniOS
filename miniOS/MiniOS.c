#include "MiniOS.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MAX_QUEUE_SIZE 100

static MessageQueue g_msgQueue;
static pthread_t g_timerThread;
static int g_timerRunning = 0;
static TimerCallback g_timerCallback = NULL;
static uint32_t g_timerInterval = 0;

static void *TimerThreadFunc(void *arg) {
    (void)arg;
    if (g_timerRunning) {
        usleep(g_timerInterval * 1000);
        if (g_timerCallback) {
            g_timerCallback();
        }
        g_timerRunning = 0;
    }
    return NULL;
}

void OS_Init(void) {
    g_msgQueue.capacity = MAX_QUEUE_SIZE;
    g_msgQueue.queue = (Message *)malloc(sizeof(Message) * g_msgQueue.capacity);
    g_msgQueue.head = 0;
    g_msgQueue.tail = 0;
    g_msgQueue.count = 0;

    pthread_mutex_init(&g_msgQueue.lock, NULL);
    pthread_cond_init(&g_msgQueue.not_empty, NULL);
}

void OS_CreateTask(void (*taskFunc)(void), const char *name) {
    (void)name;
    pthread_t thread;
    pthread_create(&thread, NULL, (void *(*)(void *))taskFunc, NULL);
    pthread_detach(thread);
}

void OS_GetMsg(Message *msg) {
    pthread_mutex_lock(&g_msgQueue.lock);
    while (g_msgQueue.count == 0) {
        pthread_cond_wait(&g_msgQueue.not_empty, &g_msgQueue.lock);
    }
    *msg = g_msgQueue.queue[g_msgQueue.head];
    g_msgQueue.head = (g_msgQueue.head + 1) % g_msgQueue.capacity;
    g_msgQueue.count--;
    pthread_mutex_unlock(&g_msgQueue.lock);
}

void OS_SendMsg(uint32_t msgId, void *data) {
    pthread_mutex_lock(&g_msgQueue.lock);
    if (g_msgQueue.count < g_msgQueue.capacity) {
        g_msgQueue.queue[g_msgQueue.tail].MsgId = msgId;
        g_msgQueue.queue[g_msgQueue.tail].Data = data;
        g_msgQueue.tail = (g_msgQueue.tail + 1) % g_msgQueue.capacity;
        g_msgQueue.count++;
        pthread_cond_signal(&g_msgQueue.not_empty);
    }
    pthread_mutex_unlock(&g_msgQueue.lock);
}

void OS_SetupTimer(TimerCallback callback, uint32_t intervalMs) {
    g_timerCallback = callback;
    g_timerInterval = intervalMs;
    g_timerRunning = 1;
    pthread_create(&g_timerThread, NULL, TimerThreadFunc, NULL);
}














