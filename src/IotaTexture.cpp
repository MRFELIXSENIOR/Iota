#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"

#include <SDL_image.h>

namespace iota {
	Texture::Texture(int width, int height): Texture(Color(255, 255, 255), width, height) {}

	Texture::Texture(const Color& color, int width, int height) {
		SDL_Surface* suf = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
		SDL_FillRect(suf, nullptr, SDL_MapRGB(suf->format, color.red, color.green, color.blue));
		texture = SDL_CreateTextureFromSurface(Window::GetCurrentWindow().GetRendererPointer(), suf);
		if (texture == nullptr)
			throw RuntimeError("Could not load texture" + std::string(SDL_GetError()));

		SDL_FreeSurface(suf);
	}

	Texture::Texture(const RenderSurface& surface) : Texture(surface.width, surface.height) {}

	Texture::Texture(const Color& color, const RenderSurface& surface): Texture(color, surface.width, surface.height) {}

	Texture::Texture(const std::string& path) : texture(IMG_LoadTexture(Window::GetCurrentWindow().GetRendererPointer(), path.c_str())) {
		if (texture == nullptr)
			throw RuntimeError("Could not load texture, " + std::string(SDL_GetError()));
	}

	Texture::Texture(const std::string& path, Window& window) : texture(IMG_LoadTexture(window.GetRendererPointer(), path.c_str())) {
		if (texture == nullptr)
			throw RuntimeError("Could not load texture, " + std::string(SDL_GetError()));
	}

	Texture::Texture(SDL_Surface* sdl_surface) : texture(SDL_CreateTextureFromSurface(Window::GetCurrentWindow().GetRendererPointer(), sdl_surface)) {
		if (texture == nullptr)
			throw RuntimeError("Could not load texture, " + std::string(SDL_GetError()));
	}

	Texture::Texture(SDL_Surface* sdl_surface, Window& window) : texture(SDL_CreateTextureFromSurface(window.GetRendererPointer(), sdl_surface)) {
		if (texture == nullptr)
			throw RuntimeError("Could not load texture, " + std::string(SDL_GetError()));
	}

	Texture::~Texture() {
		SDL_DestroyTexture(texture);
	}

	void Texture::Recolor(const Color& color) const {
		SDL_SetTextureColorMod(texture, color.red, color.green, color.blue);
	}

	SDL_Texture* Texture::GetDataPointer() const { return texture; }
}
