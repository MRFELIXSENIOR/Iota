#include <string>

#include "IotaTexture.hpp"
#include "IotaFont.hpp"
#include "IotaBasic.hpp"

namespace iota {
	Font::Font(const std::string& font_path) : Font(font_path, FontSize::MEDIUM) {}

	Font::Font(const std::string& font_path, FontSize size) : font(TTF_OpenFont(font_path.c_str(), (int)size)) {
		if (!font)
			throw RuntimeError("Could not load font, " + std::string(TTF_GetError()));
	}

	Font::~Font() { TTF_CloseFont(font); }

	void Font::RenderText(RenderSurface& surface, const std::string& str, const Color& background, const Color& foreground) {
		str8_container.push_back({ str, background, foreground, &surface });
	}

	void Font::RenderText(RenderSurface& surface, const std::string& str, const Color& fg) { RenderText(surface, str, Color(255, 255, 255), fg); }
		 
	void Font::RenderText(RenderSurface& surface, const std::u16string& str, const Color& background, const Color& foreground) {
		str16_container.push_back({ str, background, foreground, &surface }); 
	}

	void Font::RenderText(RenderSurface& surface, const std::u16string& str, const Color& fg) { RenderText(surface, str, Color(255, 255, 255), fg); }

	void Font::Load() {}
	void Font::Update(float dt) {}

	void Font::Render() {
		if (!str8_container.empty()) {
			for (auto& s : str8_container) {
				SDL_Surface* suf = TTF_RenderUTF8_LCD(font, s.text.c_str(), s.fg.GetData(), s.bg.GetData());
				if (!suf)
					throw RuntimeError("Could not render text surface, " + std::string(TTF_GetError()));

				Texture tex(suf);
				SDL_FreeSurface(suf);
				//Window::GetCurrentWindow().DrawTexture(tex, *s.surface);
			}
		}

		if (!str16_container.empty()) {
			for (auto& s : str16_container) {
				SDL_Surface* suf = TTF_RenderUNICODE_LCD(font, (const uint16_t*)s.text.c_str(), s.fg.GetData(), s.bg.GetData());
				if (!suf)
					throw RuntimeError("Could not render text surface, " + std::string(TTF_GetError()));

				Texture tex(suf);
				SDL_FreeSurface(suf);
				//Window::GetCurrentWindow().DrawTexture(tex, *s.surface);
			}
		}
	}
}
