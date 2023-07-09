#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SDL.h>

#include "IotaVector.hpp"

namespace iota {
	class RenderSurface;
	class Texture;

	struct Color final {
		uint8_t red, green, blue, alpha;

		Color();
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
		~Color();
		SDL_Color data() const;
	};

	Color GetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 0xFF);


	class Window {
	private:
		SDL_Window* window;
		friend class Renderer;
		friend class WindowManager;

	public:
		Window();
		Window(std::string window_title, unsigned int window_width, unsigned int window_height);
		~Window();

		bool Create(std::string window_title, unsigned int window_width,
			unsigned int window_height);
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

		void RenderTexture(Texture& texture);
		void RenderScreen();
		void RenderTextureToSurface(Texture& texture, RenderSurface surface);
	};

	class RenderSurface {
	private:
		SDL_Rect rect;

	public:
		RenderSurface();
		RenderSurface(int x, int y, unsigned int width, unsigned int height);
		//RenderSurface(Vector::Vec2<int> position, Vector::Vec2<unsigned int> size);
		~RenderSurface();

		//void SetPosition(Vector::Vec2<int> position);
		void SetPosition(int x, int y);
		void Resize(unsigned int width, unsigned int height);
		//void Resize(Vector::Vec2<unsigned int> size);

		SDL_Rect data() const;
	};

	namespace Basic {
		void LoadLuaSTD();
		std::vector<Texture*>& GetTextureList();
	}
} // namespace iota
