#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SDL.h>

#include "IotaVector.hpp"

namespace iota {
	class Texture;
	class RenderSurface;

	namespace Basic {
		enum class RenderingContext {
			TO_SCREEN,
			TO_RS,
		};

		enum class DrawMode {
			FILL,
			OUTLINE,
		};

		void LoadLuaSTD();

		using TextureMap = std::unordered_map<Texture*, RenderingContext>;
		const TextureMap& GetTextureMap();
	};


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

		int GetCenterX();
		int GetCenterY();

		SDL_Window* data() const;
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
		void Destroy();

		void SetDrawColor(Color color);

		void RenderTextureToScreen(Texture& texture);
		void RenderTextureToSurface(Texture& texture, RenderSurface& surface);

		void DrawRectangle(Basic::DrawMode mode, RenderSurface& surface);

		SDL_Renderer* data() const;
	};

	struct RenderSurface final {
	private:
		SDL_Rect* rect;

	public:
		RenderSurface();
		RenderSurface(int x, int y, unsigned int width, unsigned int height);
		//RenderSurface(Vector::Vec2<int> position, Vector::Vec2<unsigned int> size);
		~RenderSurface();

		//void SetPosition(Vector::Vec2<int> position);
		void SetPosition(int x, int y);
		void Resize(unsigned int width, unsigned int height);
		//void Resize(Vector::Vec2<unsigned int> size);

		SDL_Rect* data() const;
	};
} // namespace iota
