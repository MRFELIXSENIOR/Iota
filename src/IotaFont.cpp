#include <string>

#include "IotaTexture.hpp"
#include "IotaFont.hpp"

using namespace iota;

Font::Font(const std::string& font_path): Font(font_path, FontSize::MEDIUM) {}

Font::Font(const std::string& font_path, FontSize size): font_color(0, 0, 0), font(TTF_OpenFont(font_path.c_str(), (int)size)) {
	if (!font)
		throw RuntimeError("Could not load font, " + std::string(TTF_GetError()));
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
void Font::Update(float dt) {}

void Font::Render() {
	if (!str8_container.empty()) {
		for (auto& s : str8_container) {
			SDL_Surface* suf = TTF_RenderUTF8_LCD(font, s.second.c_str(), font_color.GetData(), s.first->data.color.GetData());
			if (!suf)
				throw RuntimeError("Could not render text surface, " + std::string(TTF_GetError()));

			Texture tex(suf);
			SDL_FreeSurface(suf);
			Window::GetCurrentWindow().DrawTexture(tex, *s.first);
		}
	}

	if (!str16_container.empty()) {
		for (auto& s : str16_container) {
			SDL_Surface* suf = TTF_RenderUNICODE_LCD(font, (const uint16_t*)s.second.c_str(), font_color.GetData(), s.first->data.color.GetData());
			if (!suf)
				throw RuntimeError("Could not render text surface, " + std::string(TTF_GetError()));

			Texture tex(suf);
			SDL_FreeSurface(suf);
			Window::GetCurrentWindow().DrawTexture(tex, *s.first);
		}
	}
}
