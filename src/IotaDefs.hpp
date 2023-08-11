#pragma once

#include <cstdint>
#include <SDL.h>

namespace iota {
	using PosType = int;
	using SizeType = unsigned int;

	struct Color final {
		uint8_t red, green, blue, alpha;

		Color();
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
		~Color();
		SDL_Color data() const;
	};

	Color GetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);
}