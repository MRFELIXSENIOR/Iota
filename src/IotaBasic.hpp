#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <SDL.h>

#include "IotaVector.hpp"
#include "IotaDefs.hpp"

namespace iota {
	class Texture;
	class RenderSurface;

	namespace Basic {
		enum class DrawMode {
			FILL,
			OUTLINE,
		};
	};

	enum class ObjectShape {
		RECTANGLE,
		TRIANGLE,
		CIRCLE
	};

	class Window {
	private:
		SDL_Window* window;
		friend class Renderer;

	public:
		Window();
		Window(std::string window_title, unsigned int window_width, unsigned int window_height);
		Window(SDL_Window* window);
		~Window();

		bool Create(std::string window_title, unsigned int window_width,
			unsigned int window_height);
		void Destroy();

		int GetCenterX();
		int GetCenterY();

		/*
		size: Size in 2d either width or height
		*/
		int GetRelativeCenterX(int size_x);
		int GetRelativeCenterY(int size_y);

		SDL_Window* data() const;
	};

	struct RenderSurface {
	private:
		SDL_Rect* rect;
		SDL_Surface* suf;

	public:
		RenderSurface(int x, int y, unsigned int width, unsigned int height);
		RenderSurface(Vector::Vec2<int> position, Vector::Vec2<unsigned int> size);
		~RenderSurface();

		void SetPosition(Vector::Vec2<int> position);
		void SetPosition(int x, int y);
		void Resize(unsigned int width, unsigned int height);
		void Resize(Vector::Vec2<unsigned int> size);

		SDL_Rect* data_rect() const;
		SDL_Surface* data_suf() const;

		int& x() { return rect->x; }
		int& y() { return rect->y; }
		int& width() { return rect->w; }
		int& height() { return rect->h; }

		Color color;
	};

	class Renderer {
	private:
		SDL_Renderer* renderer;
		friend class Window;

	public:
		Renderer();
		Renderer(Window& win);
		Renderer(Window& win, bool points);
		~Renderer();

		bool Create(Window& win);
		void Destroy();

		void SetDrawColor(Color color);

		void RenderTexture(Texture& texture);
		void RenderTexture(Texture& texture, RenderSurface& surface);

		void DrawRectangle(Basic::DrawMode mode, RenderSurface& surface);
		void DrawTriangle(Basic::DrawMode mode, RenderSurface& surface);
		void DrawCircle(Basic::DrawMode mode, RenderSurface& surface);

		void Draw(ObjectShape shape, Basic::DrawMode mode, RenderSurface& surface);

		SDL_Renderer* data() const;
	};
} // namespace iota
