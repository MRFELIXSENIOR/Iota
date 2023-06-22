#pragma once

#include "IotaException.hpp"
#include <functional>
#include <string>

namespace IotaEngine {
	class Renderer;
	class Window;

	namespace Application {
		using IotaMainFunction = void();

		extern bool app_running;
		extern bool app_initialized;

		bool InitializeApplication(Renderer& renderer, Window& window);
		void StartApplication();
		void IotaMain(std::function<IotaMainFunction> main_function, Renderer& renderer, Window& window);
		bool CleanApplication(Renderer& renderer, Window& window);

		void ThrowRuntimeException(std::string_view error_title, RuntimeException::RuntimeException error_code,
			std::string_view error_message);
		void ThrowRuntimeException(std::string_view error_title, RuntimeException::RuntimeException error_code);
		void ThrowRuntimeException(RuntimeException::RuntimeException error_code);

		void ThrowException(std::string_view error_title, Exception::Exception error_code, std::string_view error_message);
		void ThrowException(std::string_view error_title, Exception::Exception error_code);
		void ThrowException(Exception::Exception error_code);

	}; // namespace Application
}; // namespace IotaEngine
