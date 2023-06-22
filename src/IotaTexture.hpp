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
		Texture(Renderer& r);
		Texture(std::string_view path, Renderer& r);
		~Texture();
		SDL_Texture* data();

		Texture LoadTexture(std::string_view path, Renderer& r);
		Texture LoadTexture(std::string_view path);

		void SetRenderer(Renderer& r);
	};

	class Canvas {
		Texture texture;
	};

} // namespace IotaEngine
