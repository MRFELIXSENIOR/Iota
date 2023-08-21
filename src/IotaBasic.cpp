#include "IotaBasic.hpp"
#include "IotaException.hpp"
#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaDefs.hpp"

#include <vector>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

using namespace iota;
using namespace Basic;

Renderer::Renderer() {}
Renderer::Renderer(Window& win) {
	if (!win.data()) {
		throw RuntimeError("Cannot Create Renderer with Uninitialized Window");
	}
		renderer = SDL_CreateRenderer(win.data(), -1, SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			throw RuntimeError("Failed To Create Renderer! " + std::string(SDL_GetError()));
		}
}
Renderer::Renderer(Window& win, bool points) {
	if (!win.data() || !SDL_GetRenderer(win.data())) {
		throw RuntimeError("Failed To Create A Renderer with NULL Window or Renderer Associated Is NULL");
	}

	if (points) {
		renderer = SDL_GetRenderer(win.data());
	}
}
Renderer::~Renderer() { Destroy(); }

void Renderer::SetDrawColor(Color color) { SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha); }

// Texture

void Renderer::RenderTexture(Texture& texture) {
	SDL_RenderCopy(renderer, texture.data(), NULL, NULL);
}

void Renderer::RenderTexture(Texture& texture, RenderSurface& surface) {
	SDL_RenderCopy(renderer, texture.data(), NULL, surface.data_rect());
}

// Models

void Renderer::DrawRectangle(DrawMode mode, RenderSurface& surface) {
	switch (mode) {
	case DrawMode::FILL:
		SDL_RenderFillRect(renderer, surface.data_rect());
		break;

	case DrawMode::OUTLINE:
		SDL_RenderDrawRect(renderer, surface.data_rect());
	}
}

void Renderer::DrawTriangle(DrawMode mode, RenderSurface& surface) {
}

int RoundToMultipleOfEight(int i) {
	return (i + (8 - 1)) & -8;
}

void Renderer::DrawCircle(DrawMode mode, RenderSurface& surface) {
	switch (mode) {
	case DrawMode::FILL:
		filledCircleRGBA(renderer, (short)surface.x(), (short)surface.y(), surface.height() / 2, 0xFF, 0xFF, 0xFF, 0xFF);
		break;

	case DrawMode::OUTLINE:
		circleRGBA(renderer, (short)surface.x(), (short)surface.y(), surface.height() / 2, 0xFF, 0xFF, 0xFF, 0xFF);
		break;
	}
}

void Renderer::Draw(ObjectShape shape, DrawMode mode, RenderSurface& surface) {
	switch (shape) {
	case ObjectShape::RECTANGLE:
		DrawRectangle(mode, surface);
		break;

	case ObjectShape::TRIANGLE:
		DrawTriangle(mode, surface);
		break;

	case ObjectShape::CIRCLE:
		DrawCircle(mode, surface);
		break;
	}
}

void Renderer::Destroy() {
	SDL_DestroyRenderer(renderer);
}

SDL_Renderer* Renderer::data() const { return renderer; }

Window::Window() {}
Window::Window(std::string window_title, unsigned int window_width,
	unsigned int window_height) {
	window = SDL_CreateWindow(
		window_title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window) {
		throw RuntimeError("Failed To Create Window!" + std::string(SDL_GetError()));
	}
}
Window::Window(SDL_Window* win) {
	if (!win) {
		throw RuntimeError("Failed To Create Window With A Null Window!");
	}

	window = win;
}

Window::~Window() { Destroy(); }

SDL_Window* Window::data() const { return window; }

int Window::GetCenterX() {
	int x;
	SDL_GetWindowSize(window, &x, NULL);
	return x / 2;
}

int Window::GetCenterY() {
	int y;
	SDL_GetWindowSize(window, NULL, &y);
	return y / 2;
}

int Window::GetRelativeCenterX(int size_x) {
	return GetCenterX() - (size_x / 2);
}

int Window::GetRelativeCenterY(int size_y) {
	return GetCenterY() - (size_y / 2);
}

RenderSurface::RenderSurface(int x, int y, unsigned int width, unsigned int height) :
	rect(new SDL_Rect) {
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
	suf = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
}

RenderSurface::RenderSurface(Vector::Vec2<int> position, Vector::Vec2<unsigned int> size) :
	rect(new SDL_Rect) {
	rect->x = position.x;
	rect->y = position.y;
	rect->w = size.x;
	rect->h = size.y;
	suf = SDL_CreateRGBSurface(0, size.x, size.y, 32, 0, 0, 0, 0);
}

RenderSurface::~RenderSurface() {}

void RenderSurface::SetPosition(Vector::Vec2<int> position) {
	rect->x = position.x;
	rect->y = position.y;
}

void RenderSurface::SetPosition(int x, int y) {
	rect->x = x;
	rect->y = y;
}

void RenderSurface::Resize(Vector::Vec2<unsigned int> size) {
	rect->w = size.x;
	rect->h = size.y;
}

void RenderSurface::Resize(unsigned int width, unsigned int height) {
	rect->w = width;
	rect->h = height;
}

SDL_Rect* RenderSurface::data_rect() const { return rect; }
SDL_Surface* RenderSurface::data_suf() const { return suf; }
