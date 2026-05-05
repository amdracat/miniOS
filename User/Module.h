#ifndef MODULE_H
#define MODULE_H

typedef enum {
    EVENT_INITIALIZE,
    EVENT_SET_SPEED,
    EVENT_GET_SPEED,
    EVENT_CHANGE_MODE,
    EVENT_GET_MODE
} MainTaskEvent;

typedef struct {
    MainTaskEvent event;
    int value;
} ModuleEventData;

#define MSG_ID_MODULE_EVENT 100

void Initialize(void);
void SetSpeed(int speed);
int GetSpeed(void);
void ChangeMode(int mode);
int GetMode(void);

void MainTask_Event(MainTaskEvent event, int value);

#endif /* MODULE_H */
