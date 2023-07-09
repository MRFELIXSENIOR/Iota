#pragma once

#include <string>

struct SDL_Texture;

namespace iota {
	class Renderer;

	class Texture {
	private:
		SDL_Texture* texture;

	public:
		Texture();
		~Texture();
		SDL_Texture* data();

		Texture& LoadTexture(const std::string& path);
		Texture& LoadTextureWithRenderer(const std::string& path, Renderer& rdrer);
		static void LoadLuaSTD();
	};
} // namespace iota
