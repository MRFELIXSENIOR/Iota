#include "IotaTexture.hpp"
#include "IotaApplication.hpp"

#include "SDL_image.h"

using namespace iota;

Texture::Texture() : texture(nullptr) {}
Texture& Texture::LoadTexture(std::string_view path) {
	SDL_Texture* result;

	SDL_Surface* surface = IMG_Load(path.data());
	if (!surface)
		Application::Error("Failed To Load Texture File!",
			SDL_GetError());

	result = SDL_CreateTextureFromSurface(Application::GetRenderer().renderer, surface);
	if (!result) {
		Application::Error(
			"Failed To Create Texture From Surface",
			SDL_GetError());
	}

	SDL_FreeSurface(surface);
	texture = result;
	return *this;
}
Texture& Texture::LoadTexture(std::string_view path, Renderer& rdrer) {
	SDL_Texture* result;

	SDL_Surface* surface = IMG_Load(path.data());
	if (!surface)
		Application::Error("Failed To Load Texture File!",
			SDL_GetError());

	result = SDL_CreateTextureFromSurface(rdrer.renderer, surface);
	if (!result) {
		Application::Error(
			"Failed To Create Texture From Surface",
			SDL_GetError());
	}

	SDL_FreeSurface(surface);
	texture = result;
	return *this;
}

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::data() { return texture; }
