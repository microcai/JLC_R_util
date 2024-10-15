
#pragma once

#include <QTimer>

#include "awaitable.hpp"


namespace qtcoro
{
    using namespace ucoro;

    using ::coro_start;

    template <typename INT>
    awaitable<void> coro_delay_ms(INT ms)
    {
        co_await callback_awaitable<void>([ms](auto continuation) {
            QTimer::singleShot(std::chrono::milliseconds(ms), continuation);
        });
    }

} // namespace qtcoro

