#pragma once

#include "IotaException.hpp"
#include "IotaBasic.hpp"
#include "IotaScriptEnvironment.hpp"

#include <functional>
#include <exception>
#include <string>

namespace iota {
	namespace Application {
		bool Initialize(const std::string& window_title, int window_width, int window_height);
		void Start();

		Window& GetWindow();
		Renderer& GetRenderer();

		bool Exit();

		bool IsRunning();
		bool IsInitialized();

		void SetFrameLimit(unsigned int target);
	}; // namespace Application
}; // namespace iota
