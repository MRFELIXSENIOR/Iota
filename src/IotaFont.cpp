#include "IotaFont.hpp"
#include "IotaBasic.hpp"

#include <SDL_ttf.h>

using namespace iota;

Font::Font(const std::string& font_path) {
	font = TTF_OpenFont(font_path.c_str(), 18);
}

Font::Font(const std::string& font_path, FontSize size) {
	font = TTF_OpenFont(font_path.c_str(), (int)size);
}

Font::~Font() {
	TTF_CloseFont(font);
}

void Font::RenderText(RenderSurface& surface, const std::string& str) {
	auto&& pair = std::make_pair(&surface, str);
	str8_container.insert(pair);
}

void Font::RenderText(RenderSurface& surface, const std::u16string& str) {
	auto&& pair = std::make_pair(&surface, str);
	str16_container.insert(pair);
}

void Font::Load() {}
void Font::Update() {}

void Font::Render() {
	for (auto& s : str8_container) {
		TTF_RenderUTF8_LCD(font, s.second.c_str(), font_color.GetData(), s.first->color.GetData());
	}

	for (auto& s : str16_container) {
		TTF_RenderUNICODE_LCD(font, (const uint16_t*)s.second.c_str(), font_color.GetData(), s.first->color.GetData());
	}
}
