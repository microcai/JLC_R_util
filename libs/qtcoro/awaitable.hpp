
#pragma once

#include <QTimer>
#include <QtCore>
#include <coroutine>

namespace qtcoro
{
	// 协程包装...
	struct awaitable_detached
	{
		struct promise_type
		{
			awaitable_detached get_return_object()
			{
				return awaitable_detached{ };
			}

			std::suspend_never initial_suspend() noexcept { return {}; }
			std::suspend_never final_suspend() noexcept { return {}; }

			void unhandled_exception() { }

			void return_void() { }
		};
	};


	// inline std::coroutine_handle<>
	// corotine_context_cleanup_awaiter::await_suspend(std::coroutine_handle<coroutine_context_promise> h) noexcept
	// {

	// }

	template <typename T>
	struct awaitable;

	template <typename T>
	struct awaitable_promise;

	template <typename T>
	struct FinalAwaiter
	{
		bool await_ready() noexcept { return false; }
		void await_resume() noexcept { }
		std::coroutine_handle<> await_suspend(std::coroutine_handle<awaitable_promise<T>> h) noexcept
		{
			if (h.promise().continuation)
			{
				return h.promise().continuation;
			}
			return std::noop_coroutine();
		}
	};

	// Promise 类型实现...
	template <typename T>
	struct awaitable_promise
	{
		std::coroutine_handle<> continuation;
		T value; // 用于存储协程返回的值

		awaitable<T> get_return_object();

		void reset_handle(std::coroutine_handle<> h) { continuation = h; }

		auto initial_suspend() { return std::suspend_always{}; }

		auto final_suspend() noexcept { return FinalAwaiter<T>{}; }
		void unhandled_exception() { }

		template <typename V>
		void return_value(V&& v) noexcept
		{
			value = std::forward<V>(v);
		}
	};

	template <>
	struct awaitable_promise<void>
	{
		std::coroutine_handle<> continuation;
		awaitable<void> get_return_object();

		void reset_handle(std::coroutine_handle<> h) { continuation = h; }

		auto initial_suspend() { return std::suspend_always{}; }

		auto final_suspend() noexcept { return FinalAwaiter<void>{}; }

		void unhandled_exception() { }

		void return_void() { }
	};

	// awaitable 协程包装...
	template <typename T>
	struct awaitable
	{
		using promise_type = awaitable_promise<T>;

		awaitable(std::coroutine_handle<promise_type> h)
			: current_coro_handle_(h)
		{
		}

		~awaitable()
		{
			if (current_coro_handle_)
			{
				Q_ASSERT(current_coro_handle_.done());
				current_coro_handle_.destroy();
			}
		}

		awaitable(awaitable&& t) noexcept
			: current_coro_handle_(t.current_coro_handle_)
		{
			t.current_coro_handle_ = nullptr;
		}

		awaitable& operator=(awaitable&& t) noexcept
		{
			if (&t != this)
			{
				if (current_coro_handle_)
					current_coro_handle_.destroy();
				current_coro_handle_   = t.current_coro_handle_;
				t.current_coro_handle_ = nullptr;
			}
			return *this;
		}

		awaitable(const awaitable&)			   = delete;
		awaitable& operator=(const awaitable&) = delete;

		T operator()() { return get(); }

		T get()
		{
			if constexpr (!std::is_same_v<T, void>)
				return std::move(current_coro_handle_.promise().value);
		}

		bool await_ready() const noexcept { return false; }

		T await_resume()
		{ // 修改返回类型为 T
			if constexpr (!std::is_same_v<T, void>)
				return std::move(current_coro_handle_.promise().value);
		}

		auto await_suspend(std::coroutine_handle<> continuation)
		{
			current_coro_handle_.promise().reset_handle(continuation);
			return current_coro_handle_;
		}

		std::coroutine_handle<promise_type> current_coro_handle_;

		void detach()
		{
			auto launch_coro = [](awaitable<T> lazy) -> awaitable_detached {
					co_await lazy;
				};

			[[maybe_unused]] auto detached = launch_coro(std::move(*this));
		}
	};

	template <typename T>
	inline awaitable<T> awaitable_promise<T>::get_return_object()
	{
		return awaitable<T>{ std::coroutine_handle<awaitable_promise<T>>::from_promise(*this) };
	}

	inline awaitable<void> awaitable_promise<void>::get_return_object()
	{
		return awaitable<void>{ std::coroutine_handle<awaitable_promise<void>>::from_promise(*this) };
	}

} // namespace qtcoro

template <typename T, typename CallbackFunction>
struct CallbackAwaiter
{
public:
	CallbackAwaiter(CallbackFunction&& callback_function)
		: callback_function_(std::move(callback_function))
	{
	}

	bool await_ready() noexcept { return false; }

	void await_suspend(std::coroutine_handle<> handle)
	{
		callback_function_(
			[handle = std::move(handle), this](T t) mutable
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

template <typename CallbackFunction>
struct CallbackAwaiter<void, CallbackFunction>
{
public:
	CallbackAwaiter(CallbackFunction&& callback_function)
		: callback_function_(std::move(callback_function))
	{
	}

	bool await_ready() noexcept { return false; }

	void await_suspend(std::coroutine_handle<> handle)
	{
		callback_function_([handle = std::move(handle)]() mutable { handle.resume(); });
	}

	void await_resume() noexcept { }

private:
	CallbackFunction callback_function_;
};

template <typename T, typename callback>
CallbackAwaiter<T, callback> callback_awaitable(callback&& cb)
{
	return CallbackAwaiter<T, callback>{ std::forward<callback>(cb) };
}

template <typename INT>
qtcoro::awaitable<int> coro_delay_ms(INT ms)
{
	co_await callback_awaitable<void>([ms](auto handle) { QTimer::singleShot(std::chrono::milliseconds(ms), handle); });
	co_return ms;
}

// inline qtcoro::DetachedCoroutine coro_wapper_func(qtcoro::awaitable<void> awaitable_coro)
// {
// 	co_await callback_awaitable<void>([](auto handle) { QTimer::singleShot(std::chrono::milliseconds(0), handle); });
// 	co_await awaitable_coro;
// 	co_return;
// }

inline void start_coro(qtcoro::awaitable<void>&& awaitable_coro)
{
	awaitable_coro.detach();
}
