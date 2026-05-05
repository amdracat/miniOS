#ifndef MAIN_H
#define MAIN_H

void MainTaskFunc1(void* arg);
void MainTaskFunc2(void* arg);
void MainTaskTimer(void* arg);
void MainTaskEventFunc(void* arg);

// ユーザ定義
void OS_SendMsgMain(uint32_t msgId, void *data);
void OS_SendMsgSub(uint32_t msgId, void *data);

#endif /* MAIN_H */