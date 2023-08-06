#include "IotaFont.hpp"
#include "IotaBasic.hpp"

#include <SDL_ttf.h>

using namespace iota;

Font::Font(const std::string& font_path): GameBehavior() {
	font = TTF_OpenFont(font_path.data(), 18);
}

Font::Font(const std::string& font_path, FontSize size): GameBehavior() {
	font = TTF_OpenFont(font_path.data(), (int)size);
}

Font::~Font() {
	TTF_CloseFont(font);
}

void Font::Resize(FontSize size) {
	TTF_SetFontSize(font, (int)size);
}

void Font::RenderText(RenderSurface& surface, const std::string& str) {
	auto&& pair = std::make_pair(surface, str);
	str_container.insert(pair);
}

void Font::Load() {}
void Font::Render() {
	for (auto& s : str_container) {

	}
}

void Font::Update() {}