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

	class Window final {
	private:
		SDL_Window* self;
		SDL_Renderer* self_renderer;

	public:
		Window(std::string window_title, unsigned int window_width, unsigned int window_height);
		Window(SDL_Window* window);
		~Window();

		void SetDrawColor(Color color) const;

		void RenderTexture(Texture& texture) const;
		void RenderTexture(Texture& texture, RenderSurface& surface) const;

		void DrawRectangle(Basic::DrawMode mode, RenderSurface& surface) const;
		void DrawTriangle(Basic::DrawMode mode, RenderSurface& surface) const;
		void DrawCircle(Basic::DrawMode mode, RenderSurface& surface) const;

		void Draw(ObjectShape shape, Basic::DrawMode mode, RenderSurface& surface) const;

		int GetCenterX(int surface_size_x) const;
		int GetCenterY(int surface_size_y) const;

		struct Coordinate {
			int x, y;
		};

		const Coordinate& GetCenter(const RenderSurface& surface) const;

		static Window& GetCurrentFocusedWindow();

		SDL_Window* GetDataPointer() const;
		SDL_Renderer* GetRendererPointer() const;
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

		SDL_Rect* GetRectData() const;
		SDL_Surface* GetSurfaceData() const;

		void SetX(int x);
		int GetX() const;

		void SetY(int y);
		int GetY() const;

		void SetWidth(int width);
		int GetWidth() const;

		void SetHeight(int height);
		int GetHeight() const;

		Color color;
	};
} // namespace iota
