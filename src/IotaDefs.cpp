#include "IotaDefs.hpp"

using namespace iota;

Color::Color() : red(0xFF), green(0xFF), blue(0xFF), alpha(0xFF) {}
Color::Color(Byte r, Byte g, Byte b, Byte a) : red(r), green(g), blue(b), alpha(a) {}
SDL_Color Color::GetData() const {
	SDL_Color c = { red, green, blue, alpha };
	return c;
}