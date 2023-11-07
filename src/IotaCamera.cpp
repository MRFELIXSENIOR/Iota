#include "IotaCamera.hpp"

namespace iota {
	static Vector2<int> ws;
	static Camera* current_cam;

	Camera::Camera(Vector2<int> world_size) :
		viewport(0, 0, Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight()),
		position({ 0, 0 }),
		zoom_level(1.f),
		focused_obj(nullptr)
	{
		ws = world_size;

		current_cam = this;
	}

	void Camera::Load() {}

	void Camera::Render() {
		SDL_RenderSetScale(Window::GetCurrentWindow().GetRendererPointer(), zoom_level, zoom_level);

		Window::GetCurrentWindow().DrawRectangle(viewport, Color(0, 0, 0), false);
		SDL_RenderSetViewport(Window::GetCurrentWindow().GetRendererPointer(), viewport.GetRectPointer());
	}

	void Camera::Update(float dt) {
		if (focused_obj != nullptr) {
			position.x = focused_obj->x + focused_obj->width / 2;
			position.y = focused_obj->y + focused_obj->height / 2;
		}

		viewport.x = position.x - viewport.width / 2;
		viewport.y = position.y - viewport.height / 2;

		if (viewport.x < 0) viewport.x = 0;
		if (viewport.y < 0) viewport.y = 0;

		if (viewport.x > ws.x - viewport.width)
			viewport.x = ws.x - viewport.width;

		if (viewport.y > ws.y - viewport.height)
			viewport.y = ws.y - viewport.height;
	}

	void Camera::Focus(RenderSurface& suf) {
		focused_obj = &suf;
	}

	void Camera::Zoom(float scale) {
		zoom_level = scale;
	}

	float Camera::Zoom() const { return zoom_level; }

	Camera& Camera::GetCurrentCamera() { return *current_cam; }
}