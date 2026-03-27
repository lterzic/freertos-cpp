#pragma once

#include <FreeRTOS.h>
#include <task.h>
#include <chrono>

namespace freertos {

/**
 * Ticks are defined relative to 1 second based on the FreeRTOS config parameter.
 */
using ticks = std::chrono::duration<TickType_t, std::ratio<1, configTICK_RATE_HZ>>;

/**
 * Get the number of ticks since system start.
 */
static inline ticks get_ticks() noexcept
{
    return ticks(xTaskGetTickCount());
}

}