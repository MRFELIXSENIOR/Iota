#pragma once

#include "IotaEngine.h"
using namespace iota;


class Main : public Script {
public:
	SoundEffect sfx;
	RenderSurface surface;

	Main() :
		sfx("fart.wav"),
		surface(0, 0, 250, 250)
	{}

	void Load() {
		surface.color = Color(255, 0, 0);
		std::cout << "hi\n";

		Keyboard::GetKeydownEvent().Connect([this](KeyCode key) {
			if (key == KeyCode::F) {
				sfx.Play();
			}
		});
	}

	void Render() {
		Window::GetCurrentWindow().DrawRectangle(Basic::DrawMode::FILL, surface);
	}

	void Update() {

	}
};
