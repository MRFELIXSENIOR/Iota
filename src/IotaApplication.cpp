#include "IotaApplication.hpp"
#include "IotaBasic.hpp"
#include "IotaEvent.hpp"

#include <cstdlib>
#include <iostream>

#include "SDL.h"
#include "SDL_image.h"

using namespace IotaEngine;
using namespace Application;

Renderer* Application::app_renderer = new Renderer;
Window* Application::app_window = new Window;
bool Application::app_running = false;
static bool app_initialized = false;

bool Application::InitializeApplication(std::string_view window_title,
	int window_width, int window_height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		ThrowRuntimeException("Failed To Initialize",
			RuntimeException::INITIALIZATION_FAILURE,
			SDL_GetError());
		return false;
	}

	app_window->Create(window_title, window_width, window_height);
	app_renderer->Create(app_window);

	if (!(IMG_Init(IMG_INIT_PNG))) {
		ThrowRuntimeException("Failed To Initialize Image Loader!",
			RuntimeException::INITIALIZATION_FAILURE,
			SDL_GetError());
	}

	app_initialized = true;
	return true;
}

bool Application::CleanApplication() {
	if (app_running == false)
		return false;

	delete app_renderer;
	delete app_window;
	IMG_Quit();
	SDL_Quit();
	app_running = false;
	return true;
}

void Application::IotaMain(std::function<IotaMainFunction> main_function) {
	if (!app_initialized) {
		ThrowRuntimeException("Application Not Initialized", Application::RuntimeException::NO_INIT_ERROR);
		return;
	}

	while (app_running) {
		app_renderer->Start();
		Event::PollEvent();
		main_function();
		app_renderer->End();
	}
	CleanApplication();
}

void Application::ThrowRuntimeException(
	std::string_view error_title, RuntimeException::RuntimeException error_code,
	std::string_view error_message) {
	std::cerr << "Runtime Exception Thrown!\nException Code: " << error_code
		<< "\nTitle: " << error_title << "\nMessage: " << error_message
		<< '\n';
	CleanApplication();
	std::exit(error_code);
}

void Application::ThrowRuntimeException(
	std::string_view error_title,
	RuntimeException::RuntimeException error_code) {
	std::cerr << "Runtime Exception Thrown!\nException Code: " << error_code
		<< "\nTitle: " << error_title << '\n';
	CleanApplication();
	std::exit(error_code);
}

void Application::ThrowRuntimeException(
	RuntimeException::RuntimeException error_code) {
	std::cerr << "Runtime Exception Thrown!\nException Code: " << error_code
		<< '\n';
	CleanApplication();
	std::exit(error_code);
}

void Application::ThrowException(std::string_view error_title,
	Exception::Exception error_code,
	std::string_view error_message) {
	std::cerr << "Exception Thrown!\nException Code: " << error_code
		<< "\nTitle: " << error_title << "\nMessage: " << error_message
		<< '\n';
}

void Application::ThrowException(std::string_view error_title,
	Exception::Exception error_code) {
	std::cerr << "Exception Thrown!\nException Code: " << error_code
		<< "\nTitle: " << error_title << '\n';
}

void Application::ThrowException(Exception::Exception error_code) {
	std::cerr << "Exception Thrown!\nException Code: " << error_code << '\n';
}
