#include "../include/Task.h"
#include "../include/STK.h"

TCB tasks[MAX_TASKS];
TCB* current_task = 0;
TCB* next_task = 0;
uint8_t task_count = 0;


void sched_init()
{
    SHPR3 |= (0xFF << 16);
}


void sched_task_create(void (*task_func)(void))
{
    TCB* task = &tasks[task_count];
    task->id  = task_count;

    uint32_t* sp = &task->stack[STACK_SIZE - 1];

    *(sp)   = 0x01000000;           // xPSR
    *(--sp) = (uint32_t)task_func;  // PC    
    *(--sp) = 0xFFFFFFFD;           // LR
    *(--sp) = 0x00000000;           // R12
    *(--sp) = 0x00000000;           // R3
    *(--sp) = 0x00000000;           // R2
    *(--sp) = 0x00000000;           // R1
    *(--sp) = 0x00000000;           // R0

    // Fake software-saved registers
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

void STK_interrupt_handler()
{
    uint8_t next_id = (current_task->id + 1) % task_count;
    next_task = &tasks[next_id];   
    ICSR |= (1 << 28);
}

void sched_enable()
{
    current_task = &tasks[0];
    next_task    = &tasks[0];
    STK_enable(100000); 
    sched_start();
}