#include "IotaUIObject.hpp"
#include "IotaInput.hpp"

namespace iota {
	UIButton::UIButton() : UIButton(0, 0, 50, 50) {}

	UIButton::UIButton(int argx, int argy, int argwidth, int argheight) :
		RenderSurface(argx, argy, argwidth, argheight),
		Texture(argwidth, argheight),
		Position(x, y),
		Size(width, height),

		OnClick(click_event)
	{}

	UIButton::UIButton(const std::string& path) : UIButton(path, 0, 0, 50, 50) {}

	UIButton::UIButton(const std::string& path, int argx, int argy, int argwidth, int argheight) :
		RenderSurface(argx, argy, argwidth, argheight),
		Texture(path),
		Position(x, y),
		Size(width, height),

		OnClick(click_event)
	{}

	void UIButton::Load() {
		Mouse::OnLeftButtonDown.Connect([this](Vector2<int> pos) {
			if (pos.x >= x && pos.x <= x + width &&
				pos.y >= y && pos.y <= y + height) {
				click_event.Fire();
			}
			});
	}

	void UIButton::Render() {
		Window::GetCurrentWindow().DrawTexture(*this, *this);
	}

	void UIButton::Update(float deltatime) {

	}
}