#pragma once

#include "IotaTexture.hpp"
#include "IotaGameInstance.hpp"
#include "IotaBasic.hpp"

#include <vector>

namespace iota {
	class Sprite : Instance, RenderSurface, Texture {
	public:
		//Creates a static sprite
		Sprite(const std::string& path, int x, int y, int width, int height);

		//Creates a static sprite
		Sprite(const std::string& path);

		/*Creates an animated sprite
			sprite_sheet: the sprite animation sheet
			frame_count: amount of frames
			frame_width: frame width
			frame_height: frame height
		*/
		Sprite(const std::string& sprite_sheet, int x, int y, int width, int height, int frame_count, int frame_width = 64, int frame_height = 64);
		Sprite(const std::string& sprite_sheet, int frame_count, int frame_width = 64, int frame_height = 64);

		inline bool IsAnimated() { return is_animated; }

		ValuePointedProperty<Vector2<int>> Position;
		ValuePointedProperty<Vector2<int>> Size;

	protected:
		void Load();
		void Render();
		void Update(float dt);

	private:
		int frame_count = 0, frame_width = 0, frame_height = 0;
		int current_frame = 0;
		float elapsed_time = 0.0f;
		bool is_animated = false;

		std::vector<RenderSurface> frames;
	};
}