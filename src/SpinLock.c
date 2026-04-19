#include <stdint.h>
#include "../include/SpinLock.h"
#include "../include/Task.h"


void spinlock_acquire(lock_t* lock)
{
    uint32_t status, val;
    while(1)
    {
        val = _LDREX(lock);
        if(val == 0)
        {
            status = _STREX(lock, 1);
            if(status == 0)
            {
                _DMB();
                return;
            }
        }
    }
}

void spinlock_release(lock_t* lock)
{
    _DMB();
    *lock = 0;
}

void mutex_acquire(lock_t* lock)
{
    uint32_t status, val;
    while(1)
    {
        val = _LDREX(lock);
        if(val == 0)
        {
            status = _STREX(lock, 1);
            if(status == 0)
            {
                _DMB();
                return;
            }
        }
        sched_yield();
    }
}


void mutex_release(lock_t* lock)
{
    _DMB();
    *lock = 1;
}