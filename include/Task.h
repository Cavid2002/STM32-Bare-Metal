#ifndef TASK_H
#define TASK_H


#include <stdint.h>

#define MAX_TASKS       8
#define STACK_SIZE      256   

typedef struct
{
    volatile uint32_t* stack_ptr;
    uint32_t  stack[STACK_SIZE];
    uint8_t   id;
} TCB;

extern TCB*    current_task;
extern TCB*    next_task;
extern uint8_t task_count;
extern TCB     tasks[MAX_TASKS];

void sched_init();
void sched_task_create(void (*task_func)(void));
void sched_enable();
void sched_start();
extern void PendSV_Handler();

#define SHPR3       (*((volatile uint32_t*)0xE000ED20))  
#define ICSR        (*((volatile uint32_t*)0xE000ED04))


#endif