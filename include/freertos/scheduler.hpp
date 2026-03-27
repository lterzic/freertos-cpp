#pragma once

#include <FreeRTOS.h>
#include <task.h>

namespace freertos::scheduler {

/**
 * Possible FreeRTOS scheduler states
 */
enum class state_e {
    SUSPENDED   = taskSCHEDULER_SUSPENDED,
    NOT_STARTED = taskSCHEDULER_NOT_STARTED,
    RUNNING     = taskSCHEDULER_RUNNING,
};

/**
 * Start FreeRTOS scheduler
*/
static inline void start() noexcept
{
    vTaskStartScheduler();
}

/**
 * Return the scheduler state
 */
static inline state_e get_state() noexcept
{
    return static_cast<state_e>(xTaskGetSchedulerState());
}

}