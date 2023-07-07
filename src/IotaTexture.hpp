#pragma once

#include <string>

struct SDL_Texture;

namespace IotaEngine {
	class Renderer;
	class Texture {
	private:
		SDL_Texture* texture;

	public:
		Texture();
		~Texture();
		SDL_Texture* data();

		Texture& LoadTexture(std::string_view path, Renderer& rdrer);
		Texture& LoadTexture(std::string_view path);
	};
} // namespace IotaEngine
