#pragma once

#include "IotaGameInstance.hpp"
#include "IotaBasic.hpp"
#include "IotaProperty.hpp"
#include "IotaTexture.hpp"
#include "IotaEvent.hpp"
#include "IotaDef.hpp"

namespace iota {
	class IOTA_API UIButton : Instance, RenderSurface, Texture {
	public:
		UIButton();
		UIButton(int x, int y, int width, int height);

		UIButton(const std::string& path);
		UIButton(const std::string& path, int x, int y, int width, int height);

		ValuePointedProperty<Vector2<int>> Position;
		ValuePointedProperty<Vector2<int>> Size;

		ScriptSignal<> OnClick;

		Color RGBColor;

	protected:
		void Load();
		void Render();
		void Update(float deltatime);

	private:
		Event<> click_event;
	};
}
