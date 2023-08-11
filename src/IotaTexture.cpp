#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"

#include <SDL_image.h>

using namespace iota;

Texture::Texture() : texture(nullptr), surface(nullptr) {}
Texture& Texture::LoadTexture(const std::string& path) {
	SDL_Texture* result = IMG_LoadTexture(Application::GetRenderer().data(), path.c_str());
	if (!result) {
		Application::Throw(ErrorType::ERROR, "Texture Load Failure", SDL_GetError());
	}

	texture = result;
	return *this;
}
Texture& Texture::LoadTexture(const std::string& path, Renderer& rdrer) {
	SDL_Texture* result = IMG_LoadTexture(rdrer.data(), path.c_str());
	if (!result) {
		Application::Throw(ErrorType::ERROR, "Texture Load Failure", SDL_GetError());
	}

	texture = result;
	return *this;
}

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::data() { return texture; }
