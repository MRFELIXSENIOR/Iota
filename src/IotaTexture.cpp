#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaScriptEnvironment.hpp"
#include "IotaException.hpp"
#include "IotaBasic.hpp"

#include <SDL_image.h>

using namespace iota;

Texture::Texture() : texture(nullptr), surface(nullptr) {}
Texture& Texture::LoadTexture(const std::string& path) {
	SDL_Texture* result = IMG_LoadTexture(Application::GetRenderer().data(), path.c_str());
	if (!result) {
		Application::Error("Texture Load Failure", SDL_GetError());
	}

	texture = result;
	return *this;
}
Texture& Texture::LoadTexture(const std::string& path, Renderer& rdrer) {
	SDL_Texture* result = IMG_LoadTexture(rdrer.data(), path.c_str());
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
	sol::state& lua = Lua::GetState();
	sol::table& Iota = Lua::GetIota();

	sol::usertype<Texture> texture = lua.new_usertype<Texture>(
		"Texture",
		sol::constructors<Texture()>());

	texture.set_function("LoadTexture", sol::overload(
		static_cast<Texture& (Texture::*)(const std::string&)>(&Texture::LoadTexture),
		static_cast<Texture& (Texture::*)(const std::string&, Renderer&)>(&Texture::LoadTexture)
	));
}
