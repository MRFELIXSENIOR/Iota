#pragma once

#include "IotaVector.hpp"

#include <cstdint>
#include <string>
#include <SDL.h>

namespace iota {
	using Byte = uint8_t;
	using IVector = Vector2<signed int>;
	
	using Position = IVector;

	struct Color final {
		Byte red, green, blue, alpha;

		Color();
		Color(Byte r, Byte g, Byte b, Byte a = 0xFF);
		SDL_Color GetData() const;
	};
}