#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

#include <SDL.h>

#include "IotaDefs.hpp"
#include "IotaVector.hpp"

namespace iota {
	class Texture;
	class RenderSurface;

	namespace Basic {
		enum class DrawMode {
			FILL,
			OUTLINE,
		};

		enum class ObjectShape {
			RECTANGLE,
			TRIANGLE,
			CIRCLE
		};

		void Load();
		void Render();
		void Update();

		void PollEvent();
		void AppLoop();
	};

	class Window final {
	private:
		SDL_Window* self;
		SDL_Renderer* self_renderer;

	public:
		Window(std::string window_title, unsigned int window_width, unsigned int window_height);
		Window(SDL_Window* window);

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();

		void SetDrawColor(const Color& color) const;

		void RenderTextureToScreen(Texture& texture) const;
		void RenderTexture(Texture& texture, const RenderSurface& surface) const;
		void RenderTexture(Texture& texture, const Position& pos) const;

		void DrawRectangle(Basic::DrawMode mode, const RenderSurface& surface) const;
		void DrawTriangle(Basic::DrawMode mode, const RenderSurface& surface) const;
		void DrawCircle(Basic::DrawMode mode, const RenderSurface& surface) const;

		template <Basic::ObjectShape Shape>
		void Draw(Basic::DrawMode mode, const RenderSurface& surface) const {
			switch (Shape) {
			case Basic::ObjectShape::RECTANGLE:
				DrawRectangle(mode, surface);
				break;

			case Basic::ObjectShape::TRIANGLE:
				DrawTriangle(mode, surface);
				break;

			case Basic::ObjectShape::CIRCLE:
				DrawCircle(mode, surface);
				break;
			}
		}

		static Window& GetCurrentWindow();
		static Window& GetFocusedWindow();

		SDL_Window* GetDataPointer() const;
		SDL_Renderer* GetRendererPointer() const;
	};

	struct RenderSurface {
	private:
		SDL_Rect* rect;

	public:
		RenderSurface(int x, int y, unsigned int width, unsigned int height);
		~RenderSurface();

		void SetPosition(int x, int y);
		void Resize(unsigned int width, unsigned int height);

		SDL_Rect* GetRectData() const;

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
