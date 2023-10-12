#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <optional>

#include <SDL.h>

#include "IotaVector.hpp"
#include "IotaTexture.hpp"

namespace iota {
	class Texture;
	class RenderSurface;

	using Byte = uint8_t;

	struct Color final {
		Byte red, green, blue, alpha;

		Color();
		Color(Byte r, Byte g, Byte b, Byte a = 255U);
		SDL_Color GetData() const;
	};

	namespace Basic {
		struct RenderData {
		public:
			bool fill;
			Color color;
		};

		void Load();
		void Render();
		void Update(float delta_time);

		void PollEvent();
		void AppLoop();
	}

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

		void DrawTextureToScreen(Texture& texture) const;
		void DrawTexture(Texture& texture, const RenderSurface& surface) const;

		void DrawRectangle(const RenderSurface& surface) const;
		void DrawTriangle(const RenderSurface& surface) const;
		void DrawCircle(const RenderSurface& surface) const;

		static Window& GetCurrentWindow();
		static Window& GetFocusedWindow();

		int GetWidth() const;
		int GetHeight() const;

		SDL_Window* GetDataPointer() const;
		SDL_Renderer* GetRendererPointer() const;
	};

	struct RenderSurface {
	private:
		friend class Window;
		SDL_Rect rect;

	public:
		RenderSurface(int x, int y, int width, int height);

		SDL_Rect& GetRectData();
		const SDL_Rect* GetRectPointer() const;

		int& x;
		int& y;

		int& width;
		int& height;

		Basic::RenderData data;
	};
} // namespace iota
