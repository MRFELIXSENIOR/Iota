#pragma once

#include <string>

struct SDL_Texture;

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
		friend void Application::Start();
		friend class Renderer;

	public:
		Texture();
		~Texture();
		SDL_Texture* data();

		Texture& LoadTexture(const std::string& path);
		Texture& LoadTextureWithRenderer(const std::string& path, Renderer& rdrer);
		static void LoadLuaSTD();
	};
} // namespace iota
