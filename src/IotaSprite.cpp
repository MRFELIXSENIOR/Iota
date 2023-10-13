#include "IotaSprite.hpp"

using namespace iota;

Sprite::Sprite(const std::string& path, int argx, int argy, int argwidth, int argheight):
	Texture(path),
	RenderSurface(argx, argy, argwidth, argheight),
	Position(x, y),
	Size(width, height)
{}
Sprite::Sprite(const std::string& path): Sprite(path, 0, 0, 50, 50) {}

Sprite::Sprite(const std::string& sprite_sheet, int argx, int argy, int argwidth, int argheight, int framec, int framew, int frameh) :
	RenderSurface(argx, argy, argwidth, argheight),
	Texture(sprite_sheet),
	Position(x, y),
	Size(width, height),
	frame_count(framec), frame_width(framew), frame_height(frameh),
	is_animated(true) 
{
	for (int i = 0; i < framec; i++)
		frames.emplace_back(i * framew, 0, framew, frameh);
}
Sprite::Sprite(const std::string& sprite_sheet, int framec, int framew, int frameh) : Sprite(sprite_sheet, 0, 0, 50, 50, framec, framew, frameh) {}

void Sprite::Load() {}
void Sprite::Render() {
	Window::GetCurrentWindow().DrawTexture(*this, frames[current_frame], *this);
}

void Sprite::Update(float dt) {
	elapsed_time += dt;
	if (elapsed_time >= 0.1f) {
		elapsed_time = 0.0f;
		current_frame = (current_frame + 1) % frames.size();
	}
}
