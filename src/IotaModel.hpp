#pragma once

#include "IotaGameInstance.hpp"
#include "IotaBasic.hpp"
#include "IotaVector.hpp"

namespace iota {
	class Object : GameInstance::Instance {
	private:
		RenderSurface rs;
		ObjectShape shape;

	public:
		Object();
		Object(ObjectShape sh);
		Object(Vector::Vec2<int> pos, Vector::Vec2<unsigned int> size, ObjectShape shape);
		Object(int x, int y, unsigned int w, unsigned int h, ObjectShape shape);
		~Object();

		void SetColor(Color c);
		void SetColorRGB(uint8_t red, uint8_t green, uint8_t blue);
		Color GetColor();

		void SetX(int x);
		int GetX();

		void SetY(int y);
		int GetY();

		void SetWidth(int width);
		int GetWidth();

		void SetHeight(int height);
		int GetHeight();

		void Load() override;
		void Render() override;
		void Update() override;
	};

	class GUIObject : GameInstance::Instance {
	private:
		RenderSurface rs;

	public:

	};

	namespace Model {
		void LoadLuaSTD();
	}
}; // namespace iota
