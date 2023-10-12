#pragma once

#include "IotaTexture.hpp"
#include "IotaGameInstance.hpp"
#include "IotaBasic.hpp"
#include "IotaVector.hpp"

namespace iota {
	class Tile : Instance, RenderSurface, Texture {
	public:
		Tile(const std::string& texture_path, int x, int y, int width, int height);
		Tile(const std::string& texture_path);

		ValuePointedProperty<Vector2<int>> Position;
		ValuePointedProperty<Vector2<int>> Size;

	protected:
		void Load();
		void Render();
		void Update(float dt);
	};
}