#pragma once

#include <functional>
#include <exception>
#include <string>

#include "IotaDef.hpp"

namespace iota {
	class Window;
	class Renderer;

	namespace Application {
		void IOTA_API Initialize(const std::string& window_title, int window_width, int window_height);
		void IOTA_API Start();

		void IOTA_API Exit();

		bool IsRunning();
		bool IsInitialized();

		void SetFrameLimit(unsigned int target);
		int GetFrameLimit();
	};
};
