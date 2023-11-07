#pragma once

#include <string>
#include <SDL.h>

#include "IotaDef.hpp"

namespace iota {
	struct RenderSurface;
	struct Color;
	class Window;

	class IOTA_API Texture {
	protected:
		SDL_Texture* texture;

	public:
		//Creates a blank white texture with dimensions of 'width' and 'height'
		Texture(int width, int height);

		//Creates a blank white texture with A RenderSurface as parameter
		Texture(const RenderSurface& surface);

		//Creates a texture with dimensions of 'width' and 'height' and has the color of 'color'
		Texture(const Color& color, int width, int height);

		//Creates a texture with A RenderSurface as parameter and has the color of 'color'
		Texture(const Color& color, const RenderSurface& surface);

		Texture(const std::string& path);
		Texture(const std::string& path, Window& window);

		Texture(SDL_Surface* sdl_surface);
		Texture(SDL_Surface* sdl_surface, Window& window);

		void Recolor(const Color& color) const;

		~Texture();

		SDL_Texture* GetDataPointer() const;
	};
} // namespace iota
