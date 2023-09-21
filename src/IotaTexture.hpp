#pragma once

#include <string>
#include <SDL.h>

namespace iota {
	class Window;

	class Texture {
	private:
		SDL_Texture* texture;

	public:
		Texture();
		~Texture();
		SDL_Texture* GetDataPointer() const;

		static Texture& LoadTexture(const std::string& path);
		static Texture& LoadTexture(const std::string& path, const Window& window);
	};
} // namespace iota
