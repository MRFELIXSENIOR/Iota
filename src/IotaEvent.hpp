#pragma once

#include <sigslot/signal.hpp>

#include <concepts>
#include <type_traits>

namespace iota {
	namespace Event {
		template <typename T, typename... Args>
		concept IsCallable = requires(T t, Args... args) {
			{ t(args...) } -> std::same_as<void>;
		};

		template <typename... Args> class EventSignal {
		protected:
			sigslot::signal<Args...> signal;

		public:
			EventSignal() {}
			EventSignal(const EventSignal<Args...>&) {}
			EventSignal<Args...>& operator=(const EventSignal<Args...>&) { return *this; }

			~EventSignal() {}

			template <IsCallable<Args...> T>
			void Connect(T fn) {
				signal.connect(fn);
			}

			void Disconnect() { signal.disconnect_all(); }
			void Fire(Args... args) { signal.operator()(args...); }
		};

		void PollEvent();
	}; // namespace Event
}; // namespace iota
