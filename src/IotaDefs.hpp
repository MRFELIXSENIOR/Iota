#pragma once

#include <cstdint>
#include <string>
#include <SDL.h>

namespace iota {
	using PosType = int;
	using SizeType = uint32_t;

	struct Color final {
		uint8_t red, green, blue, alpha;

		Color();
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
		~Color();
		SDL_Color GetData() const;
	};

	Color GetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);
}