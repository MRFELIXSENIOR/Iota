#include "IotaBasic.hpp"
#include "IotaException.hpp"
#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaScriptEnvironment.hpp"

#include <vector>
#include <SDL.h>

using namespace iota;
using namespace Basic;

Color::Color() : red(0), green(0), blue(0), alpha(0) {}
Color::~Color() {}
Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha() {}
SDL_Color Color::data() const {
	SDL_Color c = { red, green, blue, alpha };
	return c;
}

Color iota::GetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
	return Color(red, green, blue, alpha);
}

Renderer::Renderer() {}
Renderer::Renderer(Window& win) {
	if (!win.data()) {
		Application::Error("Cannot Create Renderer with Uninitialized Window");
	}
	else {
		renderer = SDL_CreateRenderer(win.data(), -1, SDL_RENDERER_ACCELERATED);
		if (!renderer) {
			Application::Error(
				"Failed To Create Renderer!",
				SDL_GetError());
		}
	}
}
Renderer::Renderer(Window& win, bool points) {
	if (!win.data() || !SDL_GetRenderer(win.data())) {
		Application::Error("Failed To Create A Renderer With Null Window or Renderer Associated Is Null");
	}
	else {
		if (points) {
			renderer = SDL_GetRenderer(win.data());
		}
	}
}
Renderer::~Renderer() { Destroy(); }

bool Renderer::Create(Window& win) {
	if (!win.window) {
		Application::Error("Cannot Create Renderer with Uninitialized Window");
		return false;
	}

	renderer = SDL_CreateRenderer(win.window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		Application::Error(
			"Failed To Create Renderer!",
			SDL_GetError());
		return false;
	}
	return true;
}

void Renderer::SetDrawColor(Color color) { SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha); }

// Texture

void Renderer::RenderTexture(Texture& texture) {
	SDL_RenderCopy(renderer, texture.data(), NULL, NULL);
}

void Renderer::RenderTexture(Texture& texture, RenderSurface& surface) {
	SDL_RenderCopy(renderer, texture.data(), NULL, surface.data());
}

// Models

void Renderer::DrawRectangle(DrawMode mode, RenderSurface& surface) {
	switch (mode) {
	case DrawMode::FILL:
		SDL_RenderFillRect(renderer, surface.data());
		break;

	case DrawMode::OUTLINE:
		SDL_RenderDrawRect(renderer, surface.data());
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
		window_width, window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (!window) {
		Application::Error("Failed To Create Window!", SDL_GetError());
	}
}
Window::Window(SDL_Window* win) {
	if (!win) {
		Application::Error("Failed To Create Window With A Null Window!");
	}
	else {
		window = win;
	}
}

Window::~Window() { Destroy(); }

bool Window::Create(std::string window_title, unsigned int window_width,
	unsigned int window_height) {

	window = SDL_CreateWindow(
		window_title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (!window) {
		Application::Error("Failed To Create Window!",
			SDL_GetError());
		return false;
	}
	return true;
}

void Window::Destroy() {
	SDL_DestroyWindow(window);
}

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

RenderSurface::RenderSurface() : rect(new SDL_Rect) {}
RenderSurface::RenderSurface(int x, int y, unsigned int width, unsigned int height) :
	rect(new SDL_Rect) {
	rect->x = x;
	rect->y = y;
	rect->w = width;
	rect->h = height;
}

RenderSurface::RenderSurface(Vector::Vec2<int> position, Vector::Vec2<unsigned int> size) :
	rect(new SDL_Rect) {
	rect->x = position.x;
	rect->y = position.y;
	rect->w = size.x;
	rect->h = size.y;
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

SDL_Rect* RenderSurface::data() const { return rect; }

sol::table GetCenter(Window& self) {
	int x = self.GetCenterX();
	int y = self.GetCenterY();

	sol::table center = Lua::GetState().create_table();
	center["x"] = x;
	center["y"] = y;

	return center;
}

void Basic::LoadLuaSTD() {
	if (Application::IsInitialized()) return;
	sol::state& lua = Lua::GetState();
	sol::table& Iota = Lua::GetIota();

	sol::usertype<Color> color = lua.new_usertype<Color>("Color", sol::constructors<Color(), Color(uint8_t, uint8_t, uint8_t, uint8_t)>());
	Iota["Util"]["GetColor"] = &GetColor;

	sol::usertype<Window> window = lua.new_usertype<Window>("Window", sol::constructors<Window(), Window(std::string, unsigned int, unsigned int)>());
	window["Create"] = &Window::Create;
	window["Destroy"] = &Window::Destroy;
	window["GetCenter"] = &GetCenter;

	sol::usertype<Renderer> renderer = lua.new_usertype<Renderer>("Renderer", sol::constructors<Renderer(Window&)>());

	sol::usertype<RenderSurface> surface = lua.new_usertype<RenderSurface>(
		"RenderSurface",
		sol::constructors<RenderSurface(), RenderSurface(int, int, unsigned int, unsigned int), RenderSurface(Vector::Vec2<int>, Vector::Vec2<unsigned int>)>()
	);

	BindVectorType<int>();
	BindVectorType<unsigned int>();

	surface.set_function("SetPosition", sol::overload(
		static_cast<void (RenderSurface::*)(Vector::Vec2<int>)>(&RenderSurface::SetPosition),
		static_cast<void (RenderSurface::*)(int, int)>(&RenderSurface::SetPosition)
	));

	surface.set_function("Resize", sol::overload(
		static_cast<void (RenderSurface::*)(Vector::Vec2<unsigned int>)>(&RenderSurface::Resize),
		static_cast<void (RenderSurface::*)(unsigned int, unsigned int)>(&RenderSurface::Resize)
	));
}