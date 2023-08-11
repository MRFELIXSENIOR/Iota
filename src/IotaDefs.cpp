#include "IotaDefs.hpp"

using namespace iota;

Color::Color() : red(0), green(0), blue(0), alpha(0) {}
Color::~Color() {}
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha() {}
SDL_Color Color::data() const {
	SDL_Color c = { red, green, blue, alpha };
	return c;
}

Color iota::GetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	return Color(red, green, blue, alpha);
}
