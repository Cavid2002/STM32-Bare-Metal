#include "../include/Task.h"
#include "../include/SpinLock.h"
#include "../include/STK.h"
#include "../include/DMA.h"
#include <stddef.h>

Task tasks[MAX_TASKS];
Task* current_task = 0;
Task* next_task = 0;
uint8_t task_count = 0;


void sched_init()
{
    SHPR3 |= (0xFF << 16);
    SHPR3 |= (0x10 << 24);
}


void sched_task_create(void (*task_func)(void))
{
    Task* task = &tasks[task_count];
    task->id  = task_count;
    task->state = TASK_STATE_READY;

    uint32_t* sp = &task->stack[STACK_SIZE - 1];

    *(sp)   = 0x01000000;           // xPSR
    *(--sp) = (uint32_t)task_func;  // PC    
    *(--sp) = 0xFFFFFFFD;           // LR
    *(--sp) = 0x00000000;           // R12
    *(--sp) = 0x00000000;           // R3
    *(--sp) = 0x00000000;           // R2
    *(--sp) = 0x00000000;           // R1
    *(--sp) = 0x00000000;           // R0


    *(--sp) = 0x00000000;           // R11
    *(--sp) = 0x00000000;           // R10
    *(--sp) = 0x00000000;           // R9
    *(--sp) = 0x00000000;           // R8
    *(--sp) = 0x00000000;           // R7
    *(--sp) = 0x00000000;           // R6
    *(--sp) = 0x00000000;           // R5
    *(--sp) = 0x00000000;           // R4

    task->stack_ptr = sp;
    task_count++;
}

void sched_block()
{
    current_task->state = TASK_STATE_BLOCKED;
    while(current_task->state == TASK_STATE_BLOCKED);
}

void sched_unblock(Task* task)
{
    if(task == NULL) return;
    task->state = TASK_STATE_READY;
}

void sched_yield()
{
    _DISABLE_INTR();
    uint8_t id = current_task->id;
    uint8_t next_id = (current_task->id + 1) % task_count;
    
    while(tasks[next_id].state == TASK_STATE_BLOCKED) 
    {
        next_id = (id + 1) % task_count;
        id = next_id;
    } 

    next_task = &tasks[next_id];   
    ICSR |= (1 << 28);
    _ENABLE_INTR();
}

void STK_interrupt_handler()
{
    sched_yield();
}

void sched_enable()
{
    current_task = &tasks[0];
    next_task = &tasks[0];
    STK_enable(100000); 
    sched_start();
}