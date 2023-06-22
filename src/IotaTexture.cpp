#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaBasic.hpp"

#include "SDL.h"
#include "SDL_image.h"

using namespace IotaEngine;

Texture::Texture() {}
Texture::Texture(Renderer& r): renderer(&r) {}
Texture::Texture(std::string_view path, Renderer& r): renderer(&r) {
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

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}

Texture Texture::LoadTexture(std::string_view path) { return Texture(path, *renderer); }
Texture Texture::LoadTexture(std::string_view path, Renderer& r) { return Texture(path, r); }
SDL_Texture* Texture::data() { return texture; }

void Texture::SetRenderer(Renderer& r) { renderer = &r; }
