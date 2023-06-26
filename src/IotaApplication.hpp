#pragma once

#include "IotaException.hpp"
#include "IotaBasic.hpp"

#include <functional>
#include <string>
#include <iostream>
#include <format>
#include <typeinfo>
#include <type_traits>
#include <stdexcept>

namespace IotaEngine {
	class Renderer;
	class Window;

	namespace Application {
		using IotaMainFunction = std::function<void()>;

		bool Initialize();
		bool Initialize(Window& window, Renderer& renderer);
		bool Initialize(std::string_view window_title, int window_width, int window_height);

		void Start();
		void Main(IotaMainFunction main_function, Window& window, Renderer& renderer);
		void Main(IotaMainFunction main_function);

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

	namespace Console {
		enum class ConsoleStatus {
			CONSOLE = true,
			STDOUT = false,
		};

		ConsoleStatus SwitchStream();
		ConsoleStatus GetStatus();

		template <typename T>
		void Log(T value) {
			try {
				std::cout << value << '\n';
			}
			catch (const std::exception& e) {
				Application::ThrowException(std::format("Unable to Log Value of Type: {}", typeid(T).name()), Application::Exception::CANNOT_LOG, e.what());
			}
		}
	};

}; // namespace IotaEngine
