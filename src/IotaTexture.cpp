#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaScriptEnvironment.hpp"

#include <SDL_image.h>

using namespace iota;

Texture::Texture() : texture(nullptr) {}
Texture& Texture::LoadTexture(const std::string& path) {
	SDL_Texture* result = IMG_LoadTexture(Application::GetRenderer().renderer, path.c_str());
	if (!result) {
		Application::Error("Texture Load Failure", SDL_GetError());
	}

	texture = result;
	return *this;
}
Texture& Texture::LoadTextureWithRenderer(const std::string& path, Renderer& rdrer) {
	SDL_Texture* result = IMG_LoadTexture(rdrer.renderer, path.c_str());
	if (!result) {
		Application::Error("Texture Load Failure", SDL_GetError());
	}

	texture = result;
	return *this;
}

Texture::~Texture() {
	SDL_DestroyTexture(texture);
}

SDL_Texture* Texture::data() { return texture; }

void Texture::LoadLuaSTD() {
	if (Application::IsInitialized()) return;
	sol::state& lua = Lua::GetEngineLuaState();
	sol::table& Iota = Lua::GetIota();

	sol::usertype<Texture> texture = lua.new_usertype<Texture>(
		"Texture",
		sol::constructors<Texture()>());

	texture["LoadTexture"] = &Texture::LoadTexture;
	texture["LoadTextureWithRenderer"] = &Texture::LoadTextureWithRenderer;
}
