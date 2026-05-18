#pragma once

#include "chrono.hpp"
#include <FreeRTOS.h>
#include <task.h>

namespace freertos {

/**
 * Task stack specified by the number of words.
 */
template <size_t SIZE_WORDS>
using task_wstack = StackType_t[SIZE_WORDS];

/**
 * Task stack specified by the number of bytes, rounded up to
 * be a multiple of the stack word size;
 */
template <size_t SIZE_BYTES>
using task_bstack = task_wstack<(SIZE_BYTES + sizeof(StackType_t) - 1UL) / sizeof(StackType_t)>;

/**
 * FreeRTOS Task
 */
class task {
public:
    /**
     * Create the task and set it to the ready state. Task is ready for
     * execution as soon as the scheduler starts.
     */
    template <size_t STACK_WORDS>
    explicit task(
        const char* name,
        size_t priority,
        task_wstack<STACK_WORDS>& stack
    ) :
        m_task_handle(xTaskCreateStatic(
            &task_entry,
            name,
            STACK_WORDS,
            this,
            priority,
            stack,
            &m_task_buffer
        ))
    {}

    /* Copy operations not allowed */
    task(const task&) = delete;
    task& operator=(const task&) = delete;

    /* Move operations not allowed */
    task(task&&) = delete;
    task& operator=(task&&) = delete;

    /**
     * Destructor terminates the FreeRTOS task.
     */
    virtual ~task() noexcept
    {
        vTaskDelete(m_task_handle);
    }

    /**
     * Put the task in the suspended state.
     * @note Only way to have it continue execution is by calling `task::resume`.
     */
    void suspend() noexcept
    {
        vTaskSuspend(m_task_handle);
    }

    /**
     * Put the task in the ready state if previously suspended.
     */
    void resume() noexcept
    {
        vTaskResume(m_task_handle);
    }

    /**
     * Increment task's notification value (works like a counting semaphore).
     */
    void notify() noexcept
    {
        xTaskNotifyGive(m_task_handle);
    }

    /**
     * Increment task's notification value.
     * @todo Add argument for higher priority task woken
     */
    void notify_from_isr() noexcept
    {
        vTaskNotifyGiveFromISR(m_task_handle, NULL);
    }

protected:
    /**
     * Wait for a notification to this task.
     * @returns `true` if a notification was received before timeout
     */
    bool wait_on_notify(ticks timeout, bool clear_count = false) noexcept
    {
        return ulTaskNotifyTake(clear_count, timeout.count()) > 0;
    }

    /**
     * Sleep for a given number of ticks.
     */
    void sleep(ticks duration) noexcept
    {
        vTaskDelay(duration.count());
    }

    /**
     * Sleep relative to previous wake up time.
     * @returns `true` if wakeup was delayed, `false` otherwise
     */
    bool sleep_periodic(ticks period) noexcept
    {
        if (m_first_period) {
            m_first_period = false;
            m_prev_wakeup = xTaskGetTickCount();
        }
        return xTaskDelayUntil(&m_prev_wakeup, period.count()) == pdTRUE;
    }

    /**
     * Enter a critical section.
     * @todo Add a scoped critical section object like a scoped_lock.
     */
    void enter_critical() noexcept
    {
        taskENTER_CRITICAL();
    }

    /**
     * Exit a critical section.
     */
    void exit_critical() noexcept
    {
        taskEXIT_CRITICAL();
    }

private:
    /**
     * Task function.
     */
    virtual void run() noexcept = 0;

    /**
     * Actual function which is called by the scheduler which then calls the
     * appropriate cpp style task function to allow for passing contexts to
     * the thread.
     */
    static void task_entry(void* task_instance) noexcept
    {
        auto instance = static_cast<task*>(task_instance);
        instance->run();

        /* If the function ever exits, remove this task */
        vTaskDelete(instance->m_task_handle);
    }

private:
    StaticTask_t m_task_buffer;
    TaskHandle_t m_task_handle;

    TickType_t m_prev_wakeup;
    bool m_first_period = true;
};

}