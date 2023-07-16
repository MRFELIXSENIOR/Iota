#pragma once

#include <string>
#include <SDL.h>

#include "IotaGameInstance.hpp"

namespace iota {
	class Renderer;
	class RenderSurface;

	namespace Application {
		void Start();
	}

	class Texture {
	private:
		SDL_Texture* texture;
		RenderSurface* surface;

	public:
		Texture();
		~Texture();
		SDL_Texture* data();

		Texture& LoadTexture(const std::string& path);
		Texture& LoadTexture(const std::string& path, Renderer& rdrer);
		static void LoadLuaSTD();
	};
} // namespace iota
