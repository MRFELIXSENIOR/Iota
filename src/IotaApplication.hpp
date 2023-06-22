#pragma once

#include "IotaException.hpp"
#include <functional>
#include <string>

namespace IotaEngine {
	class Renderer;
	class Window;

	namespace Application {

		using IotaMainFunction = void();

		extern Renderer* app_renderer;
		extern Window* app_window;

		extern bool app_running;

		bool InitializeApplication(std::string_view window_title, int window_width,
			int window_height);
		void IotaMain(std::function<IotaMainFunction> main_function);
		bool CleanApplication();

		void ThrowRuntimeException(std::string_view error_title, RuntimeException::RuntimeException error_code,
			std::string_view error_message);
		void ThrowRuntimeException(std::string_view error_title, RuntimeException::RuntimeException error_code);
		void ThrowRuntimeException(RuntimeException::RuntimeException error_code);

		void ThrowException(std::string_view error_title, Exception::Exception error_code, std::string_view error_message);
		void ThrowException(std::string_view error_title, Exception::Exception error_code);
		void ThrowException(Exception::Exception error_code);

	}; // namespace Application
}; // namespace IotaEngine
