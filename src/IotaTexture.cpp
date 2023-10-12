#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"

#include <SDL_image.h>

using namespace iota;

Texture::Texture(const std::string& path): texture(IMG_LoadTexture(Window::GetCurrentWindow().GetRendererPointer(), path.c_str())) {
	if (!texture)
		throw RuntimeError("Could not load texture, " + std::string(SDL_GetError()));
}

Texture::Texture(const std::string& path, Window& window): texture(IMG_LoadTexture(window.GetRendererPointer(), path.c_str())) {
	if (!texture)
		throw RuntimeError("Could not load texture, " + std::string(SDL_GetError()));
}

Texture::Texture(SDL_Surface* sdl_surface) : texture(SDL_CreateTextureFromSurface(Window::GetCurrentWindow().GetRendererPointer(), sdl_surface)) {
	if (!texture)
		throw RuntimeError("Could not load texture, " + std::string(SDL_GetError()));
}

Texture::Texture(SDL_Surface* sdl_surface, Window& window): texture(SDL_CreateTextureFromSurface(window.GetRendererPointer(), sdl_surface)) {
	if (!texture)
		throw RuntimeError("Could not load texture, " + std::string(SDL_GetError()));
}

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::GetDataPointer() const { return texture; }
