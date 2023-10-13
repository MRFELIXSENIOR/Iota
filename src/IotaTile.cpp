#include "IotaTile.hpp"

using namespace iota;

Tile::Tile(const std::string& texture_path, int argx, int argy, int argwidth, int argheight) :
	Texture(texture_path),
	RenderSurface(argx, argy, argwidth, argheight),
	Position(x, y),
	Size(width, height)
{}

Tile::Tile(const std::string& texture_path) : Tile(texture_path, 0, 0, 50, 50) {}

void Tile::Load() {

}

void Tile::Render() {
	Window::GetCurrentWindow().DrawTexture(*this, *this);
}

void Tile::Update(float dt) {

}