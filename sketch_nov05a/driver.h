#ifndef DRIVER_H
#define DRIVER_H
#include <Arduino.h>
#include <ctime>
#include <stdint.h>
#define MAXTASK 10
#define gen_func(a) \
        void task_##a (){\
          Serial.print("task :");\
          Serial.println(a);\
        }
#define func(a) task_##a 
int start_timer();

typedef void (*func)();

typedef struct task_struct {
    void (*pTask)();

    uint32_t Delay;

    uint32_t Period;

    uint32_t id;
  } task_t;
struct task_list
{
  task_t* tSCH=new task_t[MAXTASK];
  unsigned int num =0;
}  ;




uint32_t add_task_timer(int delay, int  period, void (*pTask)());

int remove_timer(uint32_t id );

int stop_timer(void);

void timer_ISR();

int timer_dispatch();

#endif 
