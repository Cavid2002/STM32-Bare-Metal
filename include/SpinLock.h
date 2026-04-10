#ifndef SPINLCK_H
#define SPINLCK_H

#include <stdint.h>

typedef volatile uint32_t lock_t;

extern uint32_t _LDREX(lock_t* lock);
extern uint32_t _STREX(lock_t* lock, uint32_t val);
extern void _CLREX();
extern void _DMB();

extern void _DISABLE_INTR();
extern void _ENABLE_INTR();

void mutex_acquire(lock_t* lock);
void mutex_release(lock_t* lock);

#endif