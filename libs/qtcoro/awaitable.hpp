
#pragma once

#include <QTimer>
#include <QtCore>
#include <coroutine>
#include <utility>

	struct debug_coro_promise
	{

		void* operator new(std::size_t size)
		{
			void* ptr = std::malloc(size);
			if (!ptr)
			{
				throw std::bad_alloc{};
			}
			qDebug() << "promise " << ptr << " created";
			return ptr;
		}

		void operator delete(void* ptr, [[maybe_unused]] std::size_t size)
		{
			qDebug() << "promise " << ptr << " freeed";
			std::free(ptr);
		}

	};


namespace qtcoro
{
	template <typename T>
	struct promise_type_base
	{
		T value; // 用于存储协程返回的值

		template <typename V>
		void return_value(V&& v) noexcept
		{
			value = std::forward<V>(v);
		}
	};

	template <>
	struct promise_type_base<void>
	{
		constexpr void return_void() noexcept { }
	};

	// 协程包装...
	template <typename T = void>
	struct awaitable_detached
	{

		awaitable_detached(const awaitable_detached&) = delete;

		struct promise_type : public promise_type_base<T>, public debug_coro_promise
		{
			awaitable_detached get_return_object()
			{
				return awaitable_detached{this};
			}

			struct joiner_job
			{
				constexpr bool await_ready() const noexcept { return false; }

				std::coroutine_handle<> await_suspend(std::coroutine_handle<>) const noexcept
				{
					std::coroutine_handle<void> promise_handle = std::coroutine_handle<promise_type>::from_promise(*this_);

					if (this_->no_owner)
					{
						promise_handle.destroy();
					}
					if (this_->joiner)
						return this_->joiner;
					return std::noop_coroutine();
				}

				constexpr void await_resume() const noexcept {}

				promise_type* this_;
			};

			constexpr auto initial_suspend() noexcept { return std::suspend_always{}; }
			auto final_suspend() noexcept { return joiner_job{this}; }

			void unhandled_exception() { }

			std::coroutine_handle<> joiner;
			bool no_owner = false;
		};

		awaitable_detached(promise_type* promise) : promise(promise) {}
		awaitable_detached(awaitable_detached && other)
			: promise(other.promise)
		{
			other.promise = nullptr;
		}

		~awaitable_detached()
		{
			if (promise)
			{
				std::coroutine_handle<void> promise_handle = std::coroutine_handle<promise_type>::from_promise(*promise);

				if (promise_handle.done())
				{
					promise_handle.destroy();
				}
				else
				{
					std::coroutine_handle<promise_type>::from_promise(*promise).resume();
					promise->no_owner = true;
				}
			}
		}

		bool await_ready() noexcept { return false ;}

		auto await_suspend(std::coroutine_handle<> continuation) noexcept
		{
			promise->joiner = continuation;
			return std::noop_coroutine();
		}

		auto await_resume() noexcept
		{

		}

		promise_type* promise;
	};

	template <typename T>
	struct awaitable;

	template <typename T>
	struct awaitable_promise;

	// Promise 类型实现...

	// awaitable 协程包装...
	template <typename T>
	struct awaitable
	{
		struct promise_type : public promise_type_base<T>, public debug_coro_promise
		{
			std::coroutine_handle<> continuation;

			awaitable get_return_object() { return awaitable { std::coroutine_handle<promise_type>::from_promise(*this) } ; }

			void reset_handle(std::coroutine_handle<> h) { continuation = h; }

			auto initial_suspend() { return std::suspend_always{}; }

			auto final_suspend() noexcept {

				struct FinalAwaiter
				{
					bool await_ready() noexcept { return false; }
					void await_resume() noexcept { }
					std::coroutine_handle<> await_suspend(std::coroutine_handle<promise_type> h) noexcept
					{
						if (h.promise().continuation)
						{
							return h.promise().continuation;
						}
						return std::noop_coroutine();
					}
				};

				return FinalAwaiter{};
			}
			void unhandled_exception() { }

			template<typename AnyType>
			auto await_transform(awaitable<AnyType> && awaiter)
			{
				return std::move(awaiter);
			}

			template<typename CallbackFunc> requires ( std::is_invocable_r<void, CallbackFunc, std::coroutine_handle<>>::value )
			auto await_transform(CallbackFunc&& awaitee)
			{
				struct awaiter
				{
					CallbackFunc awaitee;

					constexpr bool await_ready() noexcept { return false; }
					constexpr void await_resume() noexcept {}
					void await_suspend(std::coroutine_handle<> handle) noexcept
					{
						awaitee( handle );
					}
				};

				return awaiter{awaitee};
			}

			template<typename AwaitableType>
			auto await_transform(AwaitableType&& awaiter)
			{
				return std::forward<AwaitableType>(awaiter);
			}
		};

		explicit awaitable(std::coroutine_handle<promise_type> h)
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

		bool await_ready() const noexcept { return current_coro_handle_ ? current_coro_handle_.done() : true; }

		T await_resume()
		{ // 修改返回类型为 T
			if constexpr (!std::is_same_v<T, void>)
			{
				if (current_coro_handle_)
					return std::move(current_coro_handle_.promise().value);
				std::terminate();
			}
		}

		auto await_suspend(std::coroutine_handle<> continuation)
		{
			current_coro_handle_.promise().reset_handle(continuation);
			return current_coro_handle_;
		}

		auto detach()
		{
			auto launch_coro = [](awaitable<T> lazy) -> awaitable_detached<T> {
					co_return co_await lazy;
				};

			return launch_coro(std::move(*this));
		}

		std::coroutine_handle<promise_type> current_coro_handle_;
	};

} // namespace qtcoro

template <typename INT>
qtcoro::awaitable<int> coro_delay_ms(INT ms)
{
	co_await [ms](auto continuation) {
		qDebug() << "timer setup for " << continuation.address();

		QTimer::singleShot(std::chrono::milliseconds(ms), [continuation](){
		qDebug() << "timer fired on " << continuation.address();
		continuation();
	}); };
	co_return ms;
}

template <typename T>
auto start_coro(qtcoro::awaitable<T>&& awaitable_coro)
{
	return awaitable_coro.detach();
}
