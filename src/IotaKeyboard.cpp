#include "IotaKeyboard.hpp"

using namespace iota;
using namespace Keyboard;

static std::map<SDL_Keycode, Keycode> KeycodeEntries = {
	
};

static std::map<SDL_Scancode, Scancode> ScancodeEntries;

std::map<SDL_Keycode, Keycode> Keyboard::GetKeycodeEntries() { return KeycodeEntries; }
std::map<SDL_Scancode, Scancode> Keyboard::GetScancodeEntries() { return ScancodeEntries; }