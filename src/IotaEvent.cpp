#include "IotaEvent.hpp"
#include "IotaApplication.hpp"
#include "IotaKeyboard.hpp"
#include "IotaScriptEnvironment.hpp"

#include <iostream>

#include <SDL.h>

using namespace iota;
using namespace Event;

static SDL_Event global_ev;

void Event::PollEvent() {
	if (SDL_PollEvent(&global_ev) != 0) {
		switch (global_ev.type) {
		case SDL_QUIT:
			Application::Exit();
			break;

		case SDL_KEYDOWN:
			Keyboard::HandleKeyEvent(global_ev.key, Keyboard::KeyState::DOWN);
			break;

		case SDL_KEYUP:
			Keyboard::HandleKeyEvent(global_ev.key, Keyboard::KeyState::RELEASE);
			break;
		}
	}
}
