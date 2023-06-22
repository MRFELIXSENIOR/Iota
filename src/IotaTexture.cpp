#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaBasic.hpp"

#include "SDL.h"
#include "SDL_image.h"

using namespace IotaEngine;

Texture::Texture() {}
Texture::Texture(std::string_view path) {
	SDL_Texture* result;

	SDL_Surface* surface = IMG_Load(path.data());
	if (!surface)
		Application::ThrowException("Failed To Load Texture File!",
			Application::Exception::LOAD_PATH_FAILURE,
			SDL_GetError());

	result = SDL_CreateTextureFromSurface(renderer->renderer, surface);
	if (!result) {
		Application::ThrowException(
			"Failed To Create Texture From Surface",
			Application::Exception::CREATE_TEXTURE_FAILURE, SDL_GetError());
	}

	SDL_FreeSurface(surface);
	texture = result;
}

Texture Texture::LoadTexture(std::string_view path) { return Texture(path); }
SDL_Texture* Texture::data() { return texture; }
