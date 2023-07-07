#pragma once

#include "IotaException.hpp"
#include "IotaBasic.hpp"
#include "IotaScriptEnvironment.hpp"

#include <functional>
#include <exception>
#include <string>

namespace IotaEngine {
	namespace Application {
		using IotaMainFunction = std::function<void()>;

		bool Initialize(std::string_view window_title, int window_width, int window_height);
		void Start();

		Window& GetWindow();
		Renderer& GetRenderer();

		bool Clean();

		bool IsRunning();

		void ErrorHandle(const std::exception& ex);
		void SetFrameLimit(unsigned int target);
		void test();
	}; // namespace Application
}; // namespace IotaEngine
