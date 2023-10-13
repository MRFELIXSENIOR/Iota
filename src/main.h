#pragma once

#include "IotaEngine.h"
using namespace iota;


class Main : public Script {
public:
	SoundEffect sfx;
	Tile player;

	Main() :
		sfx("fart.wav"),
		player("./cat.png")
	{}

	void Load() {
		std::cout << "hi\n";

		player.Position.GetValueChangedEvent().Connect([](Vector2<int> vec) {
			std::cout << "position changed!\n";
			});

		Keyboard::GetKeydownEvent().Connect([this](KeyCode key) {
			switch (key) {
			case KeyCode::W:
				player.Position.x += 10;
				std::cout << player.Position.x << '\n';
				std::cout << "w clicked\n";
				break;
			case KeyCode::S:
				std::cout << player.Position.x << '\n';
				break;
			case KeyCode::A:
				break;
			case KeyCode::D:
				break;
			default:
				break;
			}
		});
	}

	void Render() {
	}

	void Update(float dt) {
	}
};
