#pragma once

#include "IotaGameInstance.hpp"
#include "IotaBasic.hpp"

namespace iota {
	class Camera final : Instance {
	public:
		Camera(Vector2<int> level_size);

		void Focus(RenderSurface& suf);

		void Zoom(float scale);
		float Zoom() const;

		static Camera& GetCurrentCamera();

		Vector2<int> position;

	protected:
		void Load();
		void Render();
		void Update(float dt);

	private:
		RenderSurface* focused_obj;
		RenderSurface viewport;

		float zoom_level;
	};
}