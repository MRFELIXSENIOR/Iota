#pragma once

#include <sigslot/signal.hpp>

#include <functional>

namespace iota {
	namespace Event {
		template <typename... Args> class EventSignal {
		private:
			using FunctionCallback = std::function<void(Args...)>;
			sigslot::signal<Args...> signal;

		public:
			EventSignal() {}
			~EventSignal() {}

			void Connect(FunctionCallback fn) {
				signal.connect(fn);
			}

			void Disconnect() { signal.disconnect_all(); }
			void Fire(Args... args) { signal(args...); }
		};

		void PollEvent();
	}; // namespace Event
}; // namespace iota
