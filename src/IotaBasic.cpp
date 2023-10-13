#include "IotaBasic.hpp"
#include "IotaException.hpp"
#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaInput.hpp"
#include "IotaGameInstance.hpp"

#include <vector>
#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

using namespace iota;
using namespace Basic;

static Window* current_window = nullptr;

Color::Color() : red(0xFF), green(0xFF), blue(0xFF), alpha(0xFF) {}
Color::Color(Byte r, Byte g, Byte b, Byte a) : red(r), green(g), blue(b), alpha(a) {}
SDL_Color Color::GetData() const {
	SDL_Color c = { red, green, blue, alpha };
	return c;
}

void Basic::Load() {
	for (auto& a : ActorInterface::GetCont()) { a->Load(); }
}
void Basic::Render() {
	SDL_RenderClear(Window::GetCurrentWindow().GetRendererPointer());
	for (auto& a : ActorInterface::GetCont()) { a->Render(); }
	SDL_SetRenderDrawColor(Window::GetCurrentWindow().GetRendererPointer(), 255, 255, 255, 255);
	SDL_RenderPresent(Window::GetCurrentWindow().GetRendererPointer());
}
void Basic::Update(float delta_time) {
	for (auto& a : ActorInterface::GetCont()) {
		a->Update(delta_time);
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
			Vector2<int> p(global_ev.motion.x, global_ev.motion.y);
			Mouse::GetMouseMotionEvent().e->Fire(p);
		}
			break;

		case SDL_MOUSEBUTTONDOWN:
		{
			Vector2<int> p(global_ev.motion.x, global_ev.motion.y);
			Mouse::GetMouseDownEvent().e->Fire(p);
		}
			break;

		case SDL_MOUSEBUTTONUP:
		{
			Vector2<int> p(global_ev.motion.x, global_ev.motion.y);
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
	uint32_t frame_start, previous_frame = SDL_GetTicks();
	float frame_time, dt;
	int frame_delay = 1000 / Application::GetFrameLimit();

	Load();
	while (Application::IsRunning()) {
		frame_start = SDL_GetTicks();
		dt = (frame_start - previous_frame) / 1000.f;
		previous_frame = frame_start;

		PollEvent();
		if (!Application::IsRunning()) break;
		Update(dt);
		Render();

		frame_time = (SDL_GetTicks() - frame_start) / 1000.f;
		if (frame_time < frame_delay)
			SDL_Delay(frame_delay - frame_time);
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
		throw RuntimeError("Could not create Renderer, " + std::string(SDL_GetError()));

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

int Window::GetWidth() const {
	int w;
	SDL_GetWindowSize(self, &w, nullptr);
	return w;
}

int Window::GetHeight() const {
	int h;
	SDL_GetWindowSize(self, nullptr, &h);
	return h;
}

void Window::SetDrawColor(const Color& color) const { SDL_SetRenderDrawColor(self_renderer, color.red, color.green, color.blue, color.alpha); }

// Textures
void Window::DrawTexture(Texture& texture) const {
	SDL_RenderCopy(self_renderer, texture.GetDataPointer(), NULL, NULL);
}

void Window::DrawTexture(Texture& texture, const RenderSurface& surface) const {
	SDL_RenderCopy(self_renderer, texture.GetDataPointer(), NULL, surface.GetRectPointer());
}

void Window::DrawTexture(Texture& texture, const RenderSurface& src, const RenderSurface& dest) const {
	SDL_RenderCopy(self_renderer, texture.GetDataPointer(), src.GetRectPointer(), dest.GetRectPointer());
}

// Models
void Window::DrawRectangle(const RenderSurface& surface) const {
	SetDrawColor(surface.data.color);
	switch (surface.data.fill) {
	case true:
		SDL_RenderFillRect(self_renderer, surface.GetRectPointer());
		break;

	case false:
		SDL_RenderDrawRect(self_renderer, surface.GetRectPointer());
	}
}

void Window::DrawCircle(const RenderSurface& surface) const {
	switch (surface.data.fill) {
	case true:
		filledCircleRGBA(self_renderer, surface.x, surface.y, surface.height / 2, surface.data.color.red, surface.data.color.blue, surface.data.color.green, surface.data.color.alpha);
		break;

	case false:
		circleRGBA(self_renderer, surface.x, surface.y, surface.height / 2, surface.data.color.red, surface.data.color.blue, surface.data.color.green, surface.data.color.alpha);
		break;
	}
}

RenderSurface::RenderSurface(int x, int y, int width, int height) : 
	rect({ x, y, width, height }), 
	x(rect.x),
	y(rect.y),
	width(rect.w),
	height(rect.h)
{}

SDL_Rect& RenderSurface::GetRectData() { return rect; }
const SDL_Rect* RenderSurface::GetRectPointer() const { return &rect; }
