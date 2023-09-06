#pragma once

#include "IotaGameInstance.hpp"
#include "IotaBasic.hpp"
#include "IotaVector.hpp"

#include "IotaDefs.hpp"

namespace iota {
	class Object : protected GameInstance::Instance, public RenderSurface {
	public:
		Object();
		Object(ObjectShape sh);
		Object(Vector::Vec2<PosType> pos, Vector::Vec2<SizeType> size);
		Object(Vector::Vec2<PosType> pos, Vector::Vec2<SizeType> size, ObjectShape shape);
		Object(PosType x, PosType y, SizeType w, SizeType h);
		Object(PosType x, PosType y, SizeType w, SizeType h, ObjectShape shape);
		~Object();

		void SetColor(Color c);
		void SetColorRGB(uint8_t red, uint8_t green, uint8_t blue);
		Color GetColor() const;

		void Load() override;
		void Render() override;
		void Update() override;

		ObjectShape shape;
	};

	class GUIObject : public GameInstance::Instance, public RenderSurface {
	public:
		GUIObject();
		GUIObject(PosType x, PosType y, SizeType w, SizeType h);
		GUIObject(Vector::Vec2<PosType> pos, Vector::Vec2<SizeType> size);
		~GUIObject();
	};
}; // namespace iota
