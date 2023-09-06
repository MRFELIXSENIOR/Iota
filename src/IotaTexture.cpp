#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"

#include <SDL_image.h>

using namespace iota;

Texture::Texture() : texture(nullptr), surface(nullptr) {}
Texture& Texture::LoadTexture(const std::string& path) {
	SDL_Texture* result = IMG_LoadTexture(Window::GetCurrentFocusedWindow().GetRendererPointer(), path.c_str());
	if (!result) {
		throw RuntimeError("Texture Load Failure" + std::string(SDL_GetError()));
	}

	texture = result;
	return *this;
}
Texture& Texture::LoadTexture(const std::string& path, const Window& window) {
	SDL_Texture* result = IMG_LoadTexture(window.GetRendererPointer(), path.c_str());
	if (!result) {
		throw RuntimeError("Texture Load Failure" + std::string(SDL_GetError()));
	}

	texture = result;
	return *this;
}

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::GetDataPointer() const { return texture; }
