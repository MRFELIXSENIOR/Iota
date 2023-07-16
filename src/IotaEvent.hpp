#pragma once

#include <sigslot/signal.hpp>
#include <sol/sol.hpp>

namespace iota {
	namespace Lua {
		sol::state& GetState();
	};

	namespace Event {
		template <typename... Args> class EventSignal {
		private:
			sigslot::signal<Args...> signal;

		public:
			EventSignal() {}
			EventSignal(const EventSignal<Args...>&) {}
			EventSignal<Args...>& operator=(const EventSignal<Args...>&) { return *this; }

			~EventSignal() {}

			void Connect(std::function<void(Args...)> fn) {
				signal.connect(fn);
			}

			void Disconnect() { signal.disconnect_all(); }
			void Fire(Args... args) { signal.operator()(args...); }
		};

		void PollEvent();
		void LoadLuaSTD();

		template <typename... Args>
		sol::usertype<Event::EventSignal<Args...>> BindScriptSignal() {
			sol::state& lua = Lua::GetState();

			sol::usertype<Event::EventSignal<Args...>> script_signal = lua.new_usertype<Event::EventSignal<Args...>>("ScriptSignal", sol::no_constructor);
			script_signal["Connect"] = &Event::EventSignal<Args...>::Connect;
			script_signal["Disconnect"] = &Event::EventSignal<Args...>::Disconnect;

			return script_signal;
		}
	}; // namespace Event
}; // namespace iota
