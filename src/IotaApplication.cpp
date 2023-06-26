#include "IotaApplication.hpp"
#include "IotaBasic.hpp"
#include "IotaEvent.hpp"

#include <cstdlib>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>

#include "SDL.h"
#include "SDL_image.h"

using namespace IotaEngine;
using namespace Application;

static Application::IotaMainFunction main_fn;

static Window* global_window = nullptr;
static Renderer* global_renderer = nullptr;

static Window app_window;
static Renderer app_renderer;

static bool app_running = false;
static bool app_initialized = false;

static Console::ConsoleStatus app_console_stream = Console::ConsoleStatus::STDOUT;

static std::stringstream console_buffer;

static std::streambuf* buf;
static std::streambuf* errbuf;

static bool console_logged = false;

bool Application::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		ThrowRuntimeException("Failed To Initialize",
			RuntimeException::INITIALIZATION_FAILURE,
			SDL_GetError());
		return false;
	}

	if (!(IMG_Init(IMG_INIT_PNG))) {
		ThrowRuntimeException("Failed To Initialize Image Loader!",
			RuntimeException::INITIALIZATION_FAILURE,
			SDL_GetError());
	}

	app_initialized = true;
	return true;
}

bool Application::Initialize(Window& window, Renderer& renderer) {
	global_window = &window;
	global_renderer = &renderer;
	return Initialize();
}

bool Application::Initialize(std::string_view window_title, int window_width, int window_height) {
	app_window.Create(window_title, window_width, window_height);
	app_renderer.Create(app_window);
	return Initialize();
}

bool Application::Clean() {
	if (app_running == false)
		return false;

	BasicClean();
	IMG_Quit();
	SDL_Quit();
	app_running = false;
	return true;
}

bool Application::IsRunning() { return app_running; }

void Application::Main(IotaMainFunction main_function, Window& window, Renderer& renderer) {
	if (app_initialized == false) {
		ThrowRuntimeException("Application Not Initialized", Application::RuntimeException::NO_INIT_ERROR);
		return;
	}

	global_window = &window;
	global_renderer = &renderer;
	main_fn = main_function;
}

void Application::Main(IotaMainFunction main_function) {
	Main(main_function, app_window, app_renderer);
}

void Application::Start() {
	app_running = true;
	Console::SwitchStream();
	while (app_running == true) {
		global_renderer->Start();
		Event::PollEvent();
		main_fn();
		if (console_logged == false) {
			std::ofstream file("console_output.log");

			file << console_buffer.rdbuf();
			file.close();
			Console::SwitchStream();
			std::cout << console_buffer.str() << '\n';
			console_logged = true;
			std::cout.rdbuf(nullptr);
			std::cerr.rdbuf(nullptr);
		}
		global_renderer->End();
	}
}

Window& Application::GetWindow() { return app_window; }
Renderer& Application::GetRenderer() { return app_renderer; }

Console::ConsoleStatus Console::SwitchStream() {
	switch (app_console_stream) {
	case ConsoleStatus::STDOUT:
		buf = std::cout.rdbuf(console_buffer.rdbuf());
		errbuf = std::cerr.rdbuf(console_buffer.rdbuf());
		app_console_stream = ConsoleStatus::CONSOLE;
		return ConsoleStatus::CONSOLE;
	case ConsoleStatus::CONSOLE:
		std::cout.rdbuf(buf);
		std::cerr.rdbuf(errbuf);
		app_console_stream = ConsoleStatus::STDOUT;
		return ConsoleStatus::STDOUT;
	}
}

Console::ConsoleStatus Console::GetStatus() { return app_console_stream; }

#define index(v) static_cast<int>(v)

void Application::ThrowRuntimeException(
	std::string_view error_title, RuntimeException error_code,
	std::string_view error_message) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << index(error_code)
		<< "\nTitle: " << error_title << "\nMessage: " << error_message
		<< '\n' << "[END RUNTIME EXCEPTION]\n";
	Clean();
	std::exit(index(error_code));
}

void Application::ThrowRuntimeException(
	std::string_view error_title,
	RuntimeException error_code) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << index(error_code)
		<< "\nTitle: " << error_title << '\n' << "[END RUNTIME EXCEPTION]\n";
	Clean();
	std::exit(index(error_code));
}

void Application::ThrowRuntimeException(
	RuntimeException error_code) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << index(error_code)
		<< '\n' << "[END RUNTIME EXCEPTION]\n";
	Clean();
	std::exit(index(error_code));
}

void Application::ThrowException(std::string_view error_title,
	Exception error_code,
	std::string_view error_message) {
	std::cerr << "[Exception Thrown!]\nException Code: " << index(error_code)
		<< "\nTitle: " << error_title << "\nMessage: " << error_message
		<< '\n' << "[End Exception]\n";
}

void Application::ThrowException(std::string_view error_title,
	Exception error_code) {
	std::cerr << "[Exception Thrown!]\nException Code: " << index(error_code)
		<< "\nTitle: " << error_title << '\n' << "[End Exception]\n";
}

void Application::ThrowException(Exception error_code) {
	std::cerr << "[Exception Thrown!]\nException Code: " << index(error_code) << '\n' << "[End Exception]\n";
}
