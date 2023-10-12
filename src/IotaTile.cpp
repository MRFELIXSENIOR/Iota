#include "IotaTile.hpp"

using namespace iota;

Tile::Tile(const std::string& texture_path, int x, int y, int width, int height) :
	Texture(texture_path),
	RenderSurface(x, y, width, height),
	Position(x, y),
	Size(width, height)
{}

Tile::Tile(const std::string& texture_path) : Tile(texture_path, 0, 0, 50, 50) {}

void Tile::Load() {

}

void Tile::Render() {
	Window::GetCurrentWindow().DrawTexture(*this, *this);
	Window::GetCurrentWindow().DrawRectangle(*this);
}

void Tile::Update(float dt) {

}