#include "IotaBasic.hpp"
#include "IotaException.hpp"
#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaInput.hpp"
#include "IotaGameInstance.hpp"
#include "IotaDefs.hpp"

#include <vector>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

using namespace iota;
using namespace Basic;

static Window* current_window = nullptr;

void Basic::Load() {
	auto& ct = ActorInterface::GetCont();

	for (auto& a : ct) {
		a->Load();
	}
}

void Basic::Render() {
	auto& ct = ActorInterface::GetCont();
	SDL_RenderClear(Window::GetCurrentWindow().GetRendererPointer());
	for (auto& a : ct) {
		a->Render();
	}
	SDL_SetRenderDrawColor(Window::GetCurrentWindow().GetRendererPointer(), 255, 255, 255, 255);
	SDL_RenderPresent(Window::GetCurrentWindow().GetRendererPointer());
}

void Basic::Update() {
	auto& ct = ActorInterface::GetCont();
	for (auto& a : ct) {
		a->Update();
	}
}

void Basic::PollEvent() {
	static SDL_Event global_ev;
	if (SDL_PollEvent(&global_ev) != 0) {
		switch (global_ev.type) {
		case SDL_QUIT:
			Application::Exit();
			break;

		case SDL_KEYDOWN:
			Keyboard::GetKeydownEvent().e->Fire(Keyboard::GetKey(global_ev.key.keysym.scancode));
			break;

		case SDL_KEYUP:
			Keyboard::GetKeyupEvent().e->Fire(Keyboard::GetKey(global_ev.key.keysym.scancode));
			break;

		case SDL_MOUSEMOTION:
		{
			Position p(global_ev.motion.x, global_ev.motion.y);
			Mouse::GetMouseMotionEvent().e->Fire(p);
		}
			break;

		case SDL_MOUSEBUTTONDOWN:
		{
			Position p(global_ev.motion.x, global_ev.motion.y);
			Mouse::GetMouseDownEvent().e->Fire(p);
		}
			break;

		case SDL_MOUSEBUTTONUP:
		{
			Position p(global_ev.motion.x, global_ev.motion.y);
			Mouse::GetMouseUpEvent().e->Fire(p);
		}
			break;

		case SDL_WINDOWEVENT_FOCUS_GAINED:
			auto& cont = ActorInterface::GetCont();
			for (auto& a : cont) {
				a->Init();
			}
			break;
		}
	}
}

void Basic::AppLoop() {
	uint32_t frame_start;
	float frame_time;
	int frame_delay = 1000 / Application::GetFrameLimit();

	Load();
	while (Application::IsRunning()) {
		frame_start = SDL_GetTicks();

		PollEvent();
		if (!Application::IsRunning()) break;
		Render();

		frame_time = (SDL_GetTicks() - frame_start) / 1000.f;
		if (frame_time < frame_delay)
			SDL_Delay(frame_delay - frame_time);
		Update();
	}
}

Window::Window(std::string window_title, unsigned int window_width,
	unsigned int window_height) :
	self(SDL_CreateWindow(
		window_title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL))
{
	if (!self)
		throw RuntimeError("Could not Create Window, " + std::string(SDL_GetError()));

	self_renderer = SDL_CreateRenderer(self, -1, SDL_RENDERER_ACCELERATED);
	if (!self_renderer)
		throw RuntimeError("Could not Get Renderer, " + std::string(SDL_GetError()));
	current_window = this;
}

Window::Window(SDL_Window* win) : self(win) {
	if (!self)
		throw RuntimeError("Could not Create Window");

	self_renderer = SDL_GetRenderer(self);
	if (!self_renderer)
		throw RuntimeError("Could not Get Renderer, " + std::string(SDL_GetError()));
	current_window = this;
}

Window::~Window() {
	SDL_DestroyWindow(self);
	SDL_DestroyRenderer(self_renderer);
}

Window& Window::GetCurrentWindow() {
	if (current_window == nullptr)
		throw RuntimeError("Could not Get Current Window");
	return *current_window;
}

Window& Window::GetFocusedWindow() {
	Window* w;
	SDL_Window* wp;
	if ((wp = SDL_GetKeyboardFocus()) != nullptr) {
		w = new Window(wp);
	}
	else if ((wp = SDL_GetMouseFocus()) != nullptr) {
		w = new Window(wp);
	}
	else throw RuntimeError("No window is focused");

	return *w;
}

SDL_Renderer* Window::GetRendererPointer() const { return self_renderer; }
SDL_Window* Window::GetDataPointer() const { return self; }

void Window::SetDrawColor(const Color& color) const { SDL_SetRenderDrawColor(self_renderer, color.red, color.green, color.blue, color.alpha); }

// Textures
void Window::RenderTextureToScreen(Texture& texture) const {
	SDL_RenderCopy(self_renderer, texture.GetDataPointer(), NULL, NULL);
}

void Window::RenderTexture(Texture& texture, const RenderSurface& surface) const {
	SDL_RenderCopy(self_renderer, texture.GetDataPointer(), NULL, surface.GetRectData());
}

void Window::RenderTexture(Texture& texture, const Position& pos) const {

}

// Models
void Window::DrawRectangle(DrawMode mode, const RenderSurface& surface) const {
	SetDrawColor(surface.color);
	switch (mode) {
	case DrawMode::FILL:
		SDL_RenderFillRect(self_renderer, surface.GetRectData());
		break;

	case DrawMode::OUTLINE:
		SDL_RenderDrawRect(self_renderer, surface.GetRectData());
	}
}

void Window::DrawTriangle(DrawMode mode, const RenderSurface& surface) const {}

void Window::DrawCircle(DrawMode mode, const RenderSurface& surface) const {
	switch (mode) {
	case DrawMode::FILL:
		filledCircleRGBA(self_renderer, surface.GetX(), surface.GetY(), surface.GetHeight() / 2, surface.color.red, surface.color.blue, surface.color.green, surface.color.alpha);
		break;

	case DrawMode::OUTLINE:
		circleRGBA(self_renderer, surface.GetX(), surface.GetY(), surface.GetHeight() / 2, surface.color.red, surface.color.blue, surface.color.green, surface.color.alpha);
		break;
	}
}

RenderSurface::RenderSurface(int x, int y, unsigned int width, unsigned int height) :
	rect(new SDL_Rect) {
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
}

RenderSurface::~RenderSurface() {
	delete rect;
}

void RenderSurface::SetPosition(int x, int y) {
	rect->x = x;
	rect->y = y;
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
