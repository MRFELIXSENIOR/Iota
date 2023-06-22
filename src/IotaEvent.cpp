#include "IotaEvent.hpp"
#include "IotaApplication.hpp"

#include <iostream>

#include "SDL.h"

using namespace IotaEngine;
using namespace Event;

KeyListener::KeyListener() noexcept {}
KeyListener::KeyListener(std::initializer_list<KeyEvent*> event_list)
	: registered_events(event_list) {}

bool KeyListener::AddEvent(KeyEvent* event) {
	for (KeyEvent* keyev : registered_events) {
		if ((event != nullptr && keyev == event) || event == nullptr) {
			return false;
		}
		else {
			registered_events.push_back(event);
			break;
		}
	}
	return true;
}

void KeyListener::Fire(KeyType key, KeyState keyState) {
	KeyEvent* selected;
	for (KeyEvent* keyev : registered_events) {
		if (keyev->key == key)
			selected = keyev;
	}

	selected->Fire(keyState);
}

void KeyListener::DisconnectAll() {
	for (KeyEvent* keyev : registered_events) {
		keyev->Disconnect();
	}
}

KeyEvent::KeyEvent(KeyType key, KeyState key_state)
	: key(key), key_state(key_state) {}
KeyEvent::~KeyEvent() {}

void Event::PollEvent() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			Application::CleanApplication();
			break;
		case SDL_KEYDOWN:
			SDL_KeyboardEvent keyboardEvent = event.key;
		}
	}
}
