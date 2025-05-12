
#pragma once

#include <QTimer>
#include "awaitable.hpp"

namespace mocutil
{
// The maximum Size of a string literal is 2 GB on 32-bit and 4 GB on 64-bit
// (but the compiler is likely to give up before you get anywhere near that much)
static constexpr size_t MaxStringSize =
        (std::min)(size_t((std::numeric_limits<uint>::max)()),
                   size_t((std::numeric_limits<qsizetype>::max)()));

template <uint... Nx> constexpr size_t stringDataSizeHelper(std::integer_sequence<uint, Nx...>)
{
    // same as:
    //   return (0 + ... + Nx);
    // but not using the fold expression to avoid exceeding compiler limits
    size_t total = 0;
    uint sizes[] = { Nx... };
    for (uint n : sizes)
        total += n;
    return total;
}

template <int Count, size_t StringSize> struct StringData
{
    static_assert(StringSize <= MaxStringSize, "Meta Object data is too big");
    uint offsetsAndSizes[Count] = {};
    char stringdata0[StringSize] = {};
    constexpr StringData() = default;
};

template <uint... Nx> constexpr auto stringData(const char (&...strings)[Nx])
{
    constexpr size_t StringSize = stringDataSizeHelper<Nx...>({});
    constexpr size_t Count = 2 * sizeof...(Nx);

    StringData<Count, StringSize> result;
    const char *inputs[] = { strings... };
    uint sizes[] = { Nx... };

    uint offset = 0;
    char *output = result.stringdata0;
    for (size_t i = 0; i < sizeof...(Nx); ++i) {
        // copy the input string, including the terminating null
        uint len = sizes[i];
        for (uint j = 0; j < len; ++j)
            output[offset + j] = inputs[i][j];
        result.offsetsAndSizes[2 * i] = offset + sizeof(result.offsetsAndSizes);
        result.offsetsAndSizes[2 * i + 1] = len - 1;
        offset += len;
    }

    return result;
}

}

namespace qtcoro
{
    using namespace ucoro;

    using ::coro_start;

    template <typename INT>
    awaitable<void> coro_delay_ms(INT ms)
    {
        co_await callback_awaitable<void>([ms](auto continuation)
        {
            QTimer::singleShot(std::chrono::milliseconds(ms), [continuation]() mutable {
                continuation();
            });
        });
    }

} // namespace qtcoro

