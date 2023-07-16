#pragma once

#include <functional>
#include <exception>
#include <string>

namespace iota {
	class Window;
	class Renderer;

	namespace Application {
		bool Initialize(const std::string& window_title, int window_width, int window_height);
		void Start();

		Window& GetWindow();
		Renderer& GetRenderer();

		bool Exit();

		bool IsRunning();
		bool IsInitialized();

		void SetFrameLimit(unsigned int target);
#define CheckApplication() if (Application::IsInitialized()) return;
	}; // namespace Application
}; // namespace iota
