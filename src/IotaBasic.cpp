#include "IotaBasic.hpp"
#include "IotaException.hpp"
#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaDefs.hpp"

#include <vector>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

static iota::Window* current_window = nullptr;

using namespace iota;
using namespace Basic;

Window::Window(std::string window_title, unsigned int window_width,
	unsigned int window_height): 
	self(SDL_CreateWindow(
		window_title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL))
{
	if (!self)
		throw RuntimeError("Could not Create Window, " + std::string(SDL_GetError()));

	self_renderer = SDL_CreateRenderer(self, -1, SDL_RENDERER_ACCELERATED);
	current_window = this;
}

Window::Window(SDL_Window* win): self(win) {
	if (!self)
		throw RuntimeError("Could not Create Window");

	self_renderer = SDL_GetRenderer(self);
	current_window = this;
}

Window::~Window() {
	SDL_DestroyWindow(self);
	SDL_DestroyRenderer(self_renderer);
}

Window& Window::GetCurrentFocusedWindow() { return *current_window; }
SDL_Renderer* Window::GetRendererPointer() const { return self_renderer; }
SDL_Window* Window::GetDataPointer() const { return self; }

int Window::GetCenterX(int size_x) const { 
	int x;
	SDL_GetWindowSize(self, &x, nullptr);
	x -= (size_x / 2);
	return x; 
}

int Window::GetCenterY(int size_y) const {
	int y;
	SDL_GetWindowSize(self, nullptr, &y);
	y -= (size_y / 2);
	return y; 
}

const Window::Coordinate& Window::GetCenter(const RenderSurface& surface) const {
	Coordinate coord;
	coord.x = GetCenterX(surface.GetWidth());
	coord.y = GetCenterY(surface.GetHeight());
	return coord;
}

void Window::SetDrawColor(Color color) const { SDL_SetRenderDrawColor(self_renderer, color.red, color.green, color.blue, color.alpha); }

// Textures
void Window::RenderTexture(Texture& texture) const {
	SDL_RenderCopy(self_renderer, texture.GetDataPointer(), NULL, NULL);
}

void Window::RenderTexture(Texture& texture, RenderSurface& surface) const {
	SDL_RenderCopy(self_renderer, texture.GetDataPointer(), NULL, surface.GetRectData());
}

// Models
void Window::DrawRectangle(DrawMode mode, RenderSurface& surface) const {
	switch (mode) {
	case DrawMode::FILL:
		SDL_RenderFillRect(self_renderer, surface.GetRectData());
		break;

	case DrawMode::OUTLINE:
		SDL_RenderDrawRect(self_renderer, surface.GetRectData());
	}
}

void Window::DrawTriangle(DrawMode mode, RenderSurface& surface) const {}

inline int RoundToMultipleOfEight(int i) { return (i + (8 - 1)) & -8; }
void Window::DrawCircle(DrawMode mode, RenderSurface& surface) const {
	switch (mode) {
	case DrawMode::FILL:
		filledCircleRGBA(self_renderer, surface.GetX(), surface.GetY(), surface.GetHeight() / 2, 0xFF, 0xFF, 0xFF, 0xFF);
		break;

	case DrawMode::OUTLINE:
		circleRGBA(self_renderer, surface.GetX(), surface.GetY(), surface.GetHeight() / 2, 0xFF, 0xFF, 0xFF, 0xFF);
		break;
	}
}

void Window::Draw(ObjectShape shape, DrawMode mode, RenderSurface& surface) const {
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

void RenderSurface::SetX(int x) { rect->x = x; }
int RenderSurface::GetX() const { return rect->x; }
void RenderSurface::SetY(int y) { rect->y = y; }
int RenderSurface::GetY() const { return rect->y; }
void RenderSurface::SetWidth(int width) { rect->w = width; }
int RenderSurface::GetWidth() const { return rect->w; }
void RenderSurface::SetHeight(int height) { rect->h = height; }
int RenderSurface::GetHeight() const { return rect->h; }

SDL_Rect* RenderSurface::GetRectData() const { return rect; }
SDL_Surface* RenderSurface::GetSurfaceData() const { return suf; }
