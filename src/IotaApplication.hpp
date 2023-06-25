#pragma once

#include "IotaException.hpp"
#include "IotaBasic.hpp"

#include <functional>
#include <string>

namespace IotaEngine {
	class Renderer;
	class Window;

	namespace Application {
		using IotaMainFunction = std::function<void()>;

		bool Initialize();
		bool Initialize(Window& window, Renderer& renderer);
		bool Initialize(std::string_view window_title, int window_width, int window_height);

		void Start();
		void IotaMain(IotaMainFunction main_function, Window& window, Renderer& renderer);
		void IotaMain(IotaMainFunction main_function);

		Window& GetWindow();
		Renderer& GetRenderer();

		bool Clean();

		bool IsRunning();

		void ThrowRuntimeException(std::string_view error_title, RuntimeException error_code,
			std::string_view error_message);
		void ThrowRuntimeException(std::string_view error_title, RuntimeException error_code);
		void ThrowRuntimeException(RuntimeException error_code);

		void ThrowException(std::string_view error_title, Exception error_code, std::string_view error_message);
		void ThrowException(std::string_view error_title, Exception error_code);
		void ThrowException(Exception error_code);

	}; // namespace Application
}; // namespace IotaEngine
