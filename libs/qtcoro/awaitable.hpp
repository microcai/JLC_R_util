
#pragma once

#include <coroutine>
#include <QtCore>
#include <QTimer>

namespace qtcoro
{
template <typename T>
struct awaitable;

template <typename T>
struct Promise;

template <typename T>
struct FinalAwaiter {
    bool await_ready() noexcept { return false; }
    void await_resume() noexcept {}
    std::coroutine_handle<> await_suspend(
        std::coroutine_handle<Promise<T>> h) noexcept {
            if (h.promise().continuation)
                return h.promise().continuation;
            else
            {
                printf("::holly shit, %p destroyed!\n", h.address());
                h.destroy();
                return std::noop_coroutine();
            }
    }
};

// Promise 类型实现...
template <typename T>
struct Promise {
    std::coroutine_handle<> continuation;
    T value;                       // 用于存储协程返回的值

    awaitable<T> get_return_object();

    auto initial_suspend() { return std::suspend_always{}; }

    auto final_suspend() noexcept {
        return FinalAwaiter<T>{};
    }
    void unhandled_exception(){}
};

template <>
struct Promise<void> {
    std::coroutine_handle<> continuation;

    awaitable<void> get_return_object();

    auto initial_suspend() { return std::suspend_always{}; }

    auto final_suspend() noexcept {
        return FinalAwaiter<void>{};
    }

    void unhandled_exception(){}

    void return_void() { }
};
// awaitable 协程包装...
template <typename T>
struct awaitable {
    using promise_type = Promise<T>;
    std::coroutine_handle<promise_type> h;

    awaitable(std::coroutine_handle<promise_type> h) : h(h) {}
    ~awaitable() {
        if (h == nullptr)
            return;
        if (h.done())
        {
            h.destroy();
        }
        else
        {
            printf("::holly shit, %p alive!\n", h.address());
        }
    }

    awaitable(awaitable&& t) noexcept : h(t.h) { t.h = nullptr; }
    awaitable& operator=(awaitable&& t) noexcept {
        if (&t != this) {
            if (h)
                h.destroy();
            h = t.h;
            t.h = nullptr;
        }
        return *this;
    }

    awaitable(const awaitable&) = delete;
    awaitable& operator=(const awaitable&) = delete;

    T operator()() {
        return get();
    }

    T get() {
        if (!h.done()) {
            h.resume();
        }
        if constexpr (std::is_same_v<T, void>) {
        }
        else {
            return std::move(h.promise().value);
        }
    }

    bool await_ready() const noexcept { return false; }
    T await_resume() {  // 修改返回类型为 T
        if constexpr (std::is_same_v<T, void>) {
        }
        else {
            return std::move(h.promise().value);
        }
    }

    void await_suspend(std::coroutine_handle<> continuation) {
        h.promise().continuation = continuation;
        h.resume();
    }
};

template <typename T>
inline awaitable<T> Promise<T>::get_return_object() {
    return awaitable<T>{std::coroutine_handle<Promise<T>>::from_promise(*this)};
}

inline awaitable<void> Promise<void>::get_return_object() {
    return awaitable<void>{std::coroutine_handle<Promise<void>>::from_promise(*this)};
}

}
template<typename T, typename CallbackFunction>
struct CallbackAwaiter
{
public:
    // using CallbackFunction = std::function<void(std::function<void(T)>)>;

    CallbackAwaiter(CallbackFunction callback_function)
        : callback_function_(std::move(callback_function)) {}

    bool await_ready() noexcept { return false; }

    void await_suspend(std::coroutine_handle<> handle)
    {
        callback_function_([handle = std::move(handle), this](T t) mutable
        {
            result_ = std::move(t);
            handle.resume();
        });
    }

    T await_resume() noexcept { return std::move(result_); }

private:
    CallbackFunction callback_function_;
    T result_;
};

template<typename CallbackFunction>
struct CallbackAwaiter<void, CallbackFunction>
{
public:
    // using CallbackFunction = std::function<void(std::function<void()>)>;
    CallbackAwaiter(CallbackFunction callback_function)
        : callback_function_(std::move(callback_function)) {}

    bool await_ready() noexcept { return false; }

    void await_suspend(std::coroutine_handle<> handle) {
        callback_function_(handle);
    }
    void await_resume() noexcept { }

private:
    CallbackFunction callback_function_;
};

template<typename T, typename callback>
CallbackAwaiter<T, callback>
awaitable_to_callback(callback cb)
{
    return CallbackAwaiter<T, callback>{cb};
}

template<typename INT>
qtcoro::awaitable<void> coro_delay_ms(INT ms)
{
    co_return co_await awaitable_to_callback<void>([ms](auto handle)
    {
        QTimer::singleShot(std::chrono::milliseconds(ms), handle);
    });
}

inline qtcoro::awaitable<void> starter()
{
    qtcoro::FinalAwaiter<void> a;
    
    co_return;
}

inline void start_coro(qtcoro::awaitable<void>&& awaitable_coro)
{
    QTimer::singleShot(std::chrono::milliseconds(0), [awaitable_coro=std::move(awaitable_coro)]() mutable
    {
        awaitable_coro.get();
    });
}