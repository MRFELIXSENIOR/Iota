#pragma once

#include <string>

struct SDL_Texture;

namespace IotaEngine {
	class Renderer;

	class Texture {
	private:
		SDL_Texture* texture;
		Renderer* renderer;

	public:
		Texture();
		Texture(std::string_view path);
		~Texture();
		SDL_Texture* data();

		Texture LoadTexture(std::string_view path);
	};

	class Canvas {
		Texture texture;
	};

} // namespace IotaEngine
