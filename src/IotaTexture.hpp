#pragma once

#include <string>
#include <SDL.h>

namespace iota {
	class Window;

	class Texture {
	protected:
		SDL_Texture* texture;

	public:
		Texture(const std::string& path);
		Texture(const std::string& path, Window& window);

		Texture(SDL_Surface* sdl_surface);
		Texture(SDL_Surface* sdl_surface, Window& window);

		~Texture();

		SDL_Texture* GetDataPointer() const;
	};
} // namespace iota
