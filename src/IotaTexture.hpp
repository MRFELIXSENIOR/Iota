#pragma once

#include "IotaBasic.hpp"

#include <string>
#include "SDL.h"

namespace iota {
	class Texture {
	private:
		SDL_Texture* texture;

	public:
		Texture();
		~Texture();
		SDL_Texture* data();

		Texture& LoadTexture(std::string_view path, Renderer& rdrer);
		Texture& LoadTexture(std::string_view path);
	};
} // namespace iota
