#pragma once

#include "IotaEngine.h"
using namespace iota;

static RenderSurface surface(0, 0, 250, 250);

class Main : public Script {
public:
	void Load() {
		surface.color = Color(255, 0, 0);
		std::cout << "hi\n";

		Mouse::GetMouseDownEvent().Connect([](Position pos) -> void {
			pos.Print();
		});
	}

	void Render() {
		Window::GetCurrentWindow().DrawRectangle(Basic::DrawMode::FILL, surface);
	}

	void Update() {

	}
};
