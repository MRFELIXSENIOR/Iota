#pragma once

#include "NanoSignalSlot/nano_signal_slot.hpp"
#include <concepts>
#include <functional>
#include <initializer_list>
#include <vector>

typedef unsigned int KeyType;

namespace IotaEngine {
	namespace GameInstance {
		class Instance;
	}

	namespace Event {

		template <typename... Args> class EventSignal {
		private:
			using voidargs_function = void(Args...);
			Nano::Signal<voidargs_function> main_signal;
			friend class GameInstance::Instance;

		protected:
			void Fire(Args... args) { main_signal.fire(args...); }

		public:
			EventSignal() {}
			~EventSignal() {}
			void Connect(std::function<voidargs_function> fn) {
				main_signal.connect(fn);
			}
			void Disconnect() { main_signal.disconnect_all(); }
		};

		enum KeyState {
			KEYSTATENULL = 0,
			KEYSTATERELEASE = 1,
			KEYSTATEDOWN = 2,
		};

		using KeyStateSignal = EventSignal<KeyState>;

		struct KeyEvent : KeyStateSignal {
		private:
			friend class KeyListener;

		public:
			KeyType key = 0;
			KeyState key_state;

			KeyEvent() = default;
			KeyEvent(KeyType key, KeyState key_state);
			~KeyEvent();
		};

		class KeyListener {
			std::vector<KeyEvent*> registered_events;

		public:
			~KeyListener();
			KeyListener() noexcept;
			KeyListener(std::initializer_list<KeyEvent*> event_list);

			bool AddEvent(KeyEvent* event);
			void Fire(KeyType key, KeyState key_state);
			void DisconnectAll();
		};

		void PollEvent();

	}; // namespace Event
}; // namespace IotaEngine
