#pragma once

#include <sigslot/signal.hpp>

#include <concepts>
#include <type_traits>

namespace iota {
	namespace Basic {
		void PollEvent();
	}

	template <typename T, typename... Args>
	concept IsCallable = requires(T t, Args... args) {
		{ t(args...) };
	};

	template <typename... Args>
	class Event {
	private:
		sigslot::signal<Args...> sig;

	public:
		Event() {}
		Event(const Event&) {}
		Event& operator=(const Event&) { return *this; }

		template <typename Fn>
		requires IsCallable<Fn, Args...>
		void Connect(Fn&& fn) { sig.connect(fn); }

		void Disconnect() { sig.disconnect_all(); }
		void Fire(Args... args) { sig.operator()(std::forward<Args>(args)...); }
	};

	template <typename... Args>
	class ScriptSignal : public Event<Args...> {
	public:
		ScriptSignal(Event<Args...>& event_signal) : e(&event_signal) {}

		template <typename Fn>
		requires IsCallable<Fn, Args...>
		void Connect(Fn&& fn) {
			e->Connect(fn);
		}

		void Disconnect() { e->Disconnect(); }
		void Fire(Args... args) = delete;

	private:
		Event<Args...>* e;

		friend void Basic::PollEvent();
	};
}; // namespace iota
