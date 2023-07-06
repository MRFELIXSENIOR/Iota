#pragma once

#include "common/IotaException.hpp"
#include "common/IotaBasic.hpp"
#include "lua/IotaScriptEnvironment.hpp"

#include <functional>
#include <string>
#include <iostream>

namespace IotaEngine {
	namespace Application {
		using IotaMainFunction = std::function<void()>;

		bool Initialize(std::string_view window_title, int window_width, int window_height);
		void Start(Lua::Script& script);

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

		void SetFrameLimit(unsigned int target);
		void test() { std::cout << "hi\n"; }
	}; // namespace Application
}; // namespace IotaEngine
