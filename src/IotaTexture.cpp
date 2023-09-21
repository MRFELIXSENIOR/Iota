#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"

#include <SDL_image.h>

using namespace iota;

Texture::Texture() : texture(nullptr) {}
Texture& Texture::LoadTexture(const std::string& path) {
	Texture* t;
	SDL_Texture* result = IMG_LoadTexture(Window::GetCurrentWindow().GetRendererPointer(), path.c_str());
	if (!result)
		throw RuntimeError("Texture Load Failure" + std::string(SDL_GetError()));

	t->texture = result;
	return *t;
}
Texture& Texture::LoadTexture(const std::string& path, const Window& window) {
	Texture* t = new Texture;
	SDL_Texture* result = IMG_LoadTexture(window.GetRendererPointer(), path.c_str());
	if (!result)
		throw RuntimeError("Texture Load Failure" + std::string(SDL_GetError()));

	t->texture = result;
	return *t;
}

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::GetDataPointer() const { return texture; }
