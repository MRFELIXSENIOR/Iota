#include "IotaBasic.hpp"
#include "IotaException.hpp"
#include "IotaTexture.hpp"
#include "IotaApplication.hpp"
#include "IotaScriptEnvironment.hpp"

#include <vector>
#include <SDL.h>

using namespace iota;

std::vector<Window*> windows;
std::vector<Renderer*> renderers;
std::vector<Texture*> texture_list;

std::vector<Texture*>& Basic::GetTextureList() { return texture_list; }

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

Renderer::Renderer() : renderer(nullptr) {}
Renderer::Renderer(Window& win) {
	if (!win.window) {
		Application::Error("Cannot Create Renderer with Uninitialized Window");
	}

	renderer = SDL_CreateRenderer(win.window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		Application::Error(
			"Failed To Create Renderer!",
			SDL_GetError());
	}

	renderers.push_back(this);
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

	renderers.push_back(this);
	return true;
}

void Renderer::Start() { SDL_RenderClear(renderer); }
void Renderer::End() { SDL_RenderPresent(renderer); }

void Renderer::SetDrawColor(Color color) { SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha); }
void Renderer::RenderTexture(Texture& texture) { texture_list.push_back(&texture); }
void Renderer::RenderScreen() {
	for (Texture* t : texture_list) {
		SDL_RenderCopy(renderer, t->data(), NULL, NULL);
	} 
}
void Renderer::RenderTextureToSurface(Texture& texture, RenderSurface surface) {
	//SDL_RenderCopy(renderer, texture.data(), NULL, &surface.data());
}

void Renderer::Destroy() {
	auto it = std::find(renderers.begin(), renderers.end(), this);
	if (it != renderers.end()) {
		renderers.erase(it);
	}
	SDL_DestroyRenderer(renderer);
}

Window::Window() : window(nullptr) {}
Window::Window(std::string window_title, unsigned int window_width,
	unsigned int window_height) {
	window = SDL_CreateWindow(
		window_title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		window_width, window_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	if (!window) {
		Application::Error("Failed To Create Window!", SDL_GetError());
	}

	windows.push_back(this);
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

	windows.push_back(this);
	return true;
}

void Window::Destroy() {
	auto it = std::find(windows.begin(), windows.end(), this);
	if (it != windows.end()) {
		windows.erase(it);
	}
	SDL_DestroyWindow(window);
}

RenderSurface::RenderSurface() {}
RenderSurface::RenderSurface(int x, int y, unsigned int width, unsigned int height) {
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
}
/*
RenderSurface::RenderSurface(Vector::Vec2<int> position, Vector::Vec2<unsigned int> size) {
	rect.x = position.x;
	rect.y = position.y;
	rect.w = size.x;
	rect.h = size.y;
}
*/

RenderSurface::~RenderSurface() {}
//
//void RenderSurface::SetPosition(Vector::Vec2<int> position) {
//	rect.x = position.x;
//	rect.y = position.y;
//}

void RenderSurface::SetPosition(int x, int y) {
	rect.x = x;
	rect.y = y;
}
//
//void RenderSurface::Resize(Vector::Vec2<unsigned int> size) {
//	rect.w = size.x;
//	rect.h = size.y;
//}

void RenderSurface::Resize(unsigned int width, unsigned int height) {
	rect.w = width;
	rect.h = height;
}

SDL_Rect RenderSurface::data() const { return rect; }

void Basic::LoadLuaSTD() {
	if (Application::IsInitialized()) return;
	sol::state& lua = Lua::GetEngineLuaState();
	sol::table& Iota = Lua::GetIota();

	sol::usertype<Color> color = lua.new_usertype<Color>("Color", sol::constructors<Color(), Color(uint8_t, uint8_t, uint8_t, uint8_t)>());
	Iota["Util"]["GetColor"] = &GetColor;

	sol::usertype<Window> window = lua.new_usertype<Window>("Window", sol::constructors<Window(), Window(std::string, unsigned int, unsigned int)>());
	window["Create"] = &Window::Create;
	window["Destroy"] = &Window::Destroy;

	sol::usertype<Renderer> renderer = lua.new_usertype<Renderer>("Renderer", sol::constructors<Renderer(), Renderer(Window&)>());
	renderer["Create"] = &Renderer::Create;
	renderer["Destroy"] = &Renderer::Destroy;

	renderer["RenderTexture"] = &Renderer::RenderTexture;
	renderer["RenderTextureToSurface"] = &Renderer::RenderTextureToSurface;

	sol::usertype<RenderSurface> surface = lua.new_usertype<RenderSurface>(
		"RenderSurface",
		sol::constructors<RenderSurface(), RenderSurface(int, int, unsigned int, unsigned int)>()
	);

	surface["SetPosition"] = &RenderSurface::SetPosition;
	surface["Resize"] = &RenderSurface::Resize;
}