#include <stdint.h>
#include "../include/SpinLock.h"


void spinlock_acquire(volatile uint32_t* lock)
{
    uint32_t status, val;
    while(1)
    {
        val =_LDREX(lock);
        if(val == 0)
        {
            status = _STREX(lock, 1);
            if(status == 0)
            {
                _DMB();
                break;
            }
        }
    }
}

void spinlock_release(volatile uint32_t* lock)
{
    _DMB();
    *lock = 0;
    _DMB();
}

