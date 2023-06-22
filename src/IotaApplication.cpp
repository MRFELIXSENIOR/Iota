#include "IotaApplication.hpp"
#include "IotaBasic.hpp"
#include "IotaEvent.hpp"

#include <cstdlib>
#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

using namespace IotaEngine;
using namespace Application;

static Renderer g_renderer;
static Window g_window;

bool Application::app_running = false;
bool Application::app_initialized = false;

bool Application::InitializeApplication(Renderer& renderer, Window& window) {
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

	g_renderer = renderer;
	g_window = window;

	app_initialized = true;
	return true;
}

bool Application::CleanApplication(Renderer& renderer, Window& window) {
	if (app_running == false)
		return false;

	renderer.Destroy();
	window.Destroy();
	IMG_Quit();
	SDL_Quit();
	app_running = false;
	return true;
}

void Application::StartApplication() {
	app_running = true;
}

void Application::IotaMain(std::function<IotaMainFunction> main_function, Renderer& renderer, Window& window) {
	if (app_initialized == false) {
		ThrowRuntimeException("Application Not Initialized", Application::RuntimeException::NO_INIT_ERROR);
		return;
	}

	while (app_running == true) {
		SDL_Event event;
		renderer.Start();
		Event::PollEvent(event);
		main_function();
		renderer.End();
	}
	CleanApplication(renderer, window);
}

void Application::ThrowRuntimeException(
	std::string_view error_title, RuntimeException::RuntimeException error_code,
	std::string_view error_message) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << error_code
		<< "\nTitle: " << error_title << "\nMessage: " << error_message
		<< '\n';
	CleanApplication(g_renderer, g_window);
	std::exit(error_code);
}

void Application::ThrowRuntimeException(
	std::string_view error_title,
	RuntimeException::RuntimeException error_code) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << error_code
		<< "\nTitle: " << error_title << '\n';
	CleanApplication(g_renderer, g_window);
	std::exit(error_code);
}

void Application::ThrowRuntimeException(
	RuntimeException::RuntimeException error_code) {
	std::cerr << "[RUNTIME EXCEPTION THROWN!]\nException Code: " << error_code
		<< '\n';
	CleanApplication(g_renderer, g_window);
	std::exit(error_code);
}

void Application::ThrowException(std::string_view error_title,
	Exception::Exception error_code,
	std::string_view error_message) {
	std::cerr << "[Exception Thrown!]\nException Code: " << error_code
		<< "\nTitle: " << error_title << "\nMessage: " << error_message
		<< '\n';
}

void Application::ThrowException(std::string_view error_title,
	Exception::Exception error_code) {
	std::cerr << "[Exception Thrown!]\nException Code: " << error_code
		<< "\nTitle: " << error_title << '\n';
}

void Application::ThrowException(Exception::Exception error_code) {
	std::cerr << "[Exception Thrown!]\nException Code: " << error_code << '\n';
}
