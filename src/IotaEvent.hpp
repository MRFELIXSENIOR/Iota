#pragma once

#include "NanoSignalSlot/nano_signal_slot.hpp"
#include <concepts>
#include <functional>
#include <initializer_list>
#include <vector>

typedef unsigned int KeyType;

union SDL_Event;

namespace IotaEngine {
	namespace GameInstance {
		class InstanceCore;
		class Instance;
	}

	namespace Event {
		template <typename... Args> class EventSignal {
		private:
			Nano::Signal<void(Args...)> main_signal;

		public:
			EventSignal() {}
			~EventSignal() {}
			void Connect(std::function<void(Args...)> fn) {
				main_signal.connect(fn);
			}
			void Disconnect() { main_signal.disconnect_all(); }
			void Fire(Args... args) { main_signal.fire(args...); }
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
			friend class KeyEvent;

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
