#pragma once

#include <stdint.h>
#include <string>

struct SDL_Color;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Surface;

namespace IotaEngine {
	class Texture;
	namespace Util {

		SDL_Color& GetColor(uint8_t red, uint8_t green, uint8_t blue,
			uint8_t alpha = 0xFF);
		struct Color {
			uint8_t r, g, b, a;

			Color();
			Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);
			SDL_Color data();
		};

	} // namespace Util

	class Window {
	private:
		SDL_Window* window;
		friend class Renderer;

	public:
		Window();
		Window(std::string_view window_title, int window_width, int window_height);
		~Window();

		bool Create(std::string_view window_title, int window_width,
			int window_height);
		void Destroy();
	};

	class Renderer {
	private:
		SDL_Renderer* renderer;
		friend class Window;
		friend class Texture;

	public:
		Renderer();
		Renderer(Window& win);
		~Renderer();

		bool Create(Window& win);
		void Start();
		void End();
		void Destroy();

		void SetDrawColor(Util::Color color);
		void RenderTextureToScreen(Texture& texture);
	};

	class RenderSurface {
	private:
		SDL_Rect* rect;
		SDL_Surface* surface;

	public:
		RenderSurface();
		~RenderSurface();

		//void RenderTexture(Texture& texture);
	};

} // namespace IotaEngine
