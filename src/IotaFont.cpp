#include "IotaFont.hpp"

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

void Font::RenderText(const std::string& str) {
	str_container.push_back(str);
}

void Font::SetColor(Color c) {
	color = c;
}

void Font::Load() {}
void Font::Render() {
	for (const std::string& s : str_container) {
		TTF_RenderUTF8_Solid(font, s.data(), color.data());
	}
}

void Font::Update() {}