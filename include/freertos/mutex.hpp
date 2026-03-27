#pragma once

#include "semaphore.hpp"

namespace freertos {

/**
 * FreeRTOS mutex
 * 
 * @note This is just a convenience wrapper of a semaphore.
 */
class mutex : private semaphore {
public:
    mutex() :
        semaphore(0)
    {}

    /**
     * Lock this mutex.
     */
    bool lock(ticks timeout = ticks::max()) noexcept
    {
        return wait(timeout);
    }

    /**
     * Unlock this mutex if this thread is the owner.
     */
    bool unlock() noexcept
    {
        return signal();
    }
};

}