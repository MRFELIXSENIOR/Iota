#pragma once

#include "IotaGameInstance.hpp"
#include "IotaBasic.hpp"
#include "IotaVector.hpp"

namespace iota {
	class Object : GameInstance::Instance {
	private:
		RenderSurface rs;
		Color color;

	public:
		GameInstance::Property<int> pos_x, pos_y;
		GameInstance::Property<unsigned int> width, height;

		Object();
		Object(Vector::Vec2<int> pos, Vector::Vec2<unsigned int> size);
		Object(int x, int y, unsigned int w, unsigned int h);
		~Object();

		void SetColor(Color c);
		void SetColorRGB(uint8_t red, uint8_t green, uint8_t blue);
		Color GetColor();

		void Load() override;
		void Render() override;
		void Update() override;
	};

	namespace Model {
		void LoadLuaSTD();
	}
}; // namespace iota
