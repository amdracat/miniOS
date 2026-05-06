#include <stdlib.h>
#include <unistd.h>
#include "OsTestLayer.h"

/* ネイティブOS関数*/
#include "MiniOS.h"

/* =========================
 * 内部ジョブキュー
 * ========================= */
#define JOB_DATA_SIZE 64
typedef struct job {
    os_job_fn_t fn;
    uint16_t size;
    uint8_t data[JOB_DATA_SIZE];
    struct job *next;
} job_t;

static job_t *job_head = NULL;
static job_t *job_tail = NULL;

static uint32_t g_time_ms = 0;

/* =========================
 * API実装
 * ========================= */

void OsTestLayer_Init(void) {
#if defined (OS_TEST_LAYER_ENABLE)
    /*処理があれば記載*/
#endif /* OS_TEST_LAYER_ENABLE */
}


/* 非同期実行 */
void OsTestLayer_Post(os_job_fn_t fn, void *arg,uint16_t size) {
#if defined (OS_TEST_LAYER_ENABLE)
    job_t *job = malloc(sizeof(job_t));
    job->fn = fn;
    job->size = size;
    if (size > JOB_DATA_SIZE) {
        //abort();
    }
    if(arg != NULL){
        memcpy(job->data, arg, size);
    }
    job->next = NULL;

    if (job_tail) {
        job_tail->next = job;
    } else {
        job_head = job;
    }
    job_tail = job;
#endif
}


/* =========================
 * タイマー管理
 * ========================= */

typedef struct timer {
    uint32_t trigger_time;
    os_job_fn_t fn;
    void *arg;
    struct timer *next;
} timer_t;

static timer_t *timer_head = NULL;


void OsTestLayer_SetTimer(uint32_t delay_ms, os_job_fn_t fn, void *arg) {
#if defined (OS_TEST_LAYER_ENABLE)
    timer_t *timer = malloc(sizeof(timer_t));
    timer->trigger_time = g_time_ms + delay_ms;
    timer->fn = fn;
    timer->arg = arg;
    timer->next = timer_head;
    timer_head = timer;
#else
    /* OSのネイティブタイマーを呼び出す */
    OS_SetupTimer(fn, delay_ms, arg);
#endif /* OS_TEST_LAYER_ENABLE */
}

/* 手動スケジューリング（OS_MODE_MANUAL） */

void OsTestLayer_Sim_RunOne(void)
{
#if defined (OS_TEST_LAYER_ENABLE)
    if (job_head == NULL) {
        return;
    }

    job_t *job = job_head;
    job_head = job->next;
    if (job_head == NULL) {
        job_tail = NULL;
    }
    job->fn(job->data);
    free(job);
#endif /* OS_TEST_LAYER_ENABLE */
}

void OsTestLayer_Sim_RunAll(void)
{
#if defined (OS_TEST_LAYER_ENABLE)
    while (OsTestLayer_Sim_HasPending()) {
        OsTestLayer_Sim_RunOne();
    }
#endif /* OS_TEST_LAYER_ENABLE */
}

bool OsTestLayer_Sim_HasPending(void)
{
#if defined (OS_TEST_LAYER_ENABLE)
    bool has = (job_head != NULL);
    return has;
#else
    return false;
#endif /* OS_TEST_LAYER_ENABLE */
}

uint32_t OsTestLayer_Sim_NowMs(void)
{
#if defined (OS_TEST_LAYER_ENABLE)
    return g_time_ms;
#else
    return 0;
#endif /* OS_TEST_LAYER_ENABLE */
}

void OsTestLayer_Sim_AdvanceTime(uint32_t ms)
{
#if defined (OS_TEST_LAYER_ENABLE)
    uint32_t new_time = g_time_ms + ms;
    g_time_ms = new_time;

    // 期限切れのタイマーをジョブキューに投入
    timer_t **pp = &timer_head;
    while (*pp) {
        timer_t *timer = *pp;
        if (timer->trigger_time <= new_time) {
            OsTestLayer_Post(timer->fn, timer->arg,sizeof(timer->arg));
            *pp = timer->next;
            free(timer);
        } else {
            pp = &timer->next;
        }
    }
#else
    (void)ms;
#endif /* OS_TEST_LAYER_ENABLE */
}
