#pragma once

#include "IotaTexture.hpp"

#include <stdint.h>
#include <string>
#include <vector>

struct SDL_Color;
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Surface;

namespace iota {

	SDL_Color GetColor(uint8_t red, uint8_t green, uint8_t blue,
		uint8_t alpha = 0xFF);
	struct Color {
		uint8_t red, green, blue, alpha;

		Color();
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
		SDL_Color data();
	};


	class Window {
	private:
		SDL_Window* window;
		friend class Renderer;
		friend class WindowManager;

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

		void SetDrawColor(Color color);
		void RenderTextureToScreen(Texture& texture);
	};

	class WindowManager {
	private:
		using WindowContainer = std::vector<Window*>;
		using RendererContainer = std::vector<Renderer*>;
		friend class Window;
		friend class Renderer;
		WindowContainer windows;
		RendererContainer renderers;

	public:
		WindowManager();
		~WindowManager();

		void RemoveWindow(Window* win);
		void RemoveRenderer(Renderer* rdrer);

		void AddWindow(Window* win);
		void AddRenderer(Renderer* rdrer);

		void Clean();
	};

	namespace Application {
		void BasicClean();
		void Clean(Window& win, Renderer& rdrer);
	};
} // namespace iota
