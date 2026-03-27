#pragma once

#include "chrono.hpp"
#include <FreeRTOS.h>
#include <semphr.h>

namespace freertos {

/**
 * FreeRTOS semaphore
 */
class semaphore {
public:
    /**
     * Semaphore type depends on the provided max count
     * For mutex set `max_count` to 0, for binary set to 1,
     * and higher values will create a counting semaphore
     */
    explicit semaphore(size_t max_count = 1) noexcept
    {
        if (max_count == 0)
            m_semaphore_handle = xSemaphoreCreateMutexStatic(&m_semaphore_buffer);
        else if (max_count == 1)
            m_semaphore_handle = xSemaphoreCreateBinaryStatic(&m_semaphore_buffer);
        else
            m_semaphore_handle = xSemaphoreCreateCountingStatic(max_count, 0, &m_semaphore_buffer);
    }

    virtual ~semaphore()
    {
        vSemaphoreDelete(m_semaphore_handle);
    }

    /* Copy operations not allowed */
    semaphore(const semaphore&) = delete;
    semaphore& operator=(const semaphore&) = delete;

    /* Move operations not allowed */
    semaphore(semaphore&&) = delete;
    semaphore& operator=(semaphore&&) = delete;

    /**
     * Semaphore take
     * @todo Can add checking if the scheduler has started and returning
     * `true` if not since that means there can be only 1 thread running
    */
    bool wait(ticks timeout) noexcept
    {
        return xSemaphoreTake(m_semaphore_handle, timeout.count()) == pdTRUE;
    }

    /**
     * Semaphore give
    */
    bool signal() noexcept
    {
        return xSemaphoreGive(m_semaphore_handle) == pdTRUE;
    }

    /**
     * Semaphore give from interrupt routine
     * @note Use this to make sure that task which was
     * interrupted does not block accidentally
     * @todo Can add higher prio task woken
     */
    bool signal_from_isr() noexcept
    {
        return xSemaphoreGiveFromISR(m_semaphore_handle, NULL) == pdTRUE;
    }

private:
    StaticSemaphore_t m_semaphore_buffer;
    SemaphoreHandle_t m_semaphore_handle;

};

}