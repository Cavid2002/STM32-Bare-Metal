#ifndef SPINLCK_H
#define SPINLCK_H

#include <stdint.h>

extern uint32_t _LDREX(volatile uint32_t* lock);
extern uint32_t _STREX(volatile uint32_t* lock, uint32_t val);
extern void _DMB();

void spinlock_acquire(volatile uint32_t* lock);
void spinlock_release(volatile uint32_t* lock);

#endif