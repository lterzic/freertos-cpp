#pragma once

#include "chrono.hpp"
#include <FreeRTOS.h>
#include <queue.h>

namespace freertos {

/**
 * FreeRTOS queue
 */
template <typename item_type, size_t CAPACITY>
class queue {
    static_assert(std::is_trivially_copyable_v<item_type>);
    static_assert(std::is_trivially_destructible_v<item_type>);
public:
    queue() noexcept :
        m_queue_handle(xQueueCreateStatic(CAPACITY, sizeof(item_type), m_storage, &m_queue_buffer))
    {}

    ~queue()
    {
        vQueueDelete(m_queue_handle);
    }

    /* Copy operations not allowed */
    queue(const queue&) = delete;
    queue& operator=(const queue&) = delete;

    /* Move operations not allowed */
    queue(queue&&) = delete;
    queue& operator=(queue&&) = delete;

    /**
     * Queue send
     */
    bool send(const item_type& item, ticks timeout) noexcept
    {
        return xQueueSend(m_queue_handle, &item, timeout.count()) == pdTRUE;
    }

    /**
     * Queue send from ISR
     */
    bool send_from_isr(const item_type& item) noexcept
    {
        return xQueueSendFromISR(m_queue_handle, &item, NULL) == pdTRUE;
    }

    /**
     * Receive item from queue
     */
    bool receive(item_type& buffer, ticks timeout) noexcept
    {
        return xQueueReceive(m_queue_handle, &buffer, timeout.count()) == pdTRUE;
    }

    /**
     * Peek queue
     * @note Like receive, but doesn't remove the item
     * from the queue
     */
    bool peek(item_type& buffer, ticks timeout) noexcept
    {
        return xQueuePeek(m_queue_handle, &buffer, timeout.count()) == pdTRUE;
    }

private:
    QueueHandle_t m_queue_handle;
    StaticQueue_t m_queue_buffer;

    alignas(item_type) uint8_t m_storage[CAPACITY * sizeof(item_type)];

};

}