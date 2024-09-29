
#pragma once

#include <QTimer>
#include <QtCore>
#include <coroutine>

namespace qtcoro
{
	struct coroutine_context;
	struct coroutine_context_promise;
	struct corotine_context_cleanup_awaiter
	{
		bool await_ready() noexcept { return false; }
		void await_resume() noexcept { }
		std::coroutine_handle<> await_suspend(std::coroutine_handle<coroutine_context_promise> h) noexcept
		{
			printf("::holly shit, %p destroyed!\n", h.address());
			h.destroy();
			return std::noop_coroutine();
		}
	};

	struct coroutine_context_promise
	{
		coroutine_context get_return_object();

		auto initial_suspend() { return std::suspend_always{}; }

		auto final_suspend() noexcept { return corotine_context_cleanup_awaiter{}; }

		void unhandled_exception() { }

		void return_void() { }
	};

	// 协程包装...
	struct coroutine_context
	{
		using promise_type = coroutine_context_promise;

		coroutine_context(std::coroutine_handle<promise_type> h)
			: current_coro_handle_(h)
		{
		}

		~coroutine_context() { }

		coroutine_context(coroutine_context&& t) noexcept
			: current_coro_handle_(t.current_coro_handle_)
		{
			t.current_coro_handle_ = nullptr;
		}

		coroutine_context& operator=(coroutine_context&& t) noexcept
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

		coroutine_context(const coroutine_context&)			   = delete;
		coroutine_context& operator=(const coroutine_context&) = delete;

		void shedule() { current_coro_handle_.resume(); }

		bool await_ready() const noexcept { return false; }

		void await_resume() { }

		void await_suspend(std::coroutine_handle<> continuation) { }

		std::coroutine_handle<promise_type> current_coro_handle_;
	};

	inline coroutine_context coroutine_context_promise::get_return_object()
	{
		return coroutine_context{ std::coroutine_handle<coroutine_context_promise>::from_promise(*this) };
	}

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

inline qtcoro::coroutine_context coro_wapper_func(qtcoro::awaitable<void> awaitable_coro)
{
	co_await callback_awaitable<void>([](auto handle) { QTimer::singleShot(std::chrono::milliseconds(0), handle); });
	co_await awaitable_coro;
	co_return;
}

inline void start_coro(qtcoro::awaitable<void>&& awaitable_coro)
{
	QTimer::singleShot(std::chrono::milliseconds(0),
		[awaitable_coro = std::move(awaitable_coro)]() mutable
		{
			qtcoro::coroutine_context coro_wapper = coro_wapper_func(std::move(awaitable_coro));
			coro_wapper.shedule();
		});
}
