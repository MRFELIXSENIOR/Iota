#pragma once

#include "IotaGameInstance.hpp"
#include "IotaDefs.hpp"

#include <string>
#include <unordered_map>
#include <SDL_ttf.h>

namespace iota {
	class RenderSurface;

	enum class FontSize {
		SMALL = 12,
		MEDIUM = 18,
		LARGE = 24
	};

	class Font : public ActorInterface {
	private:
		std::unordered_map<RenderSurface*, std::string> str8_container;
		std::unordered_map<RenderSurface*, std::u16string> str16_container;

		Color font_color;
		TTF_Font* font;

	public:
		using ActorInterface::ActorInterface;
		Font(const std::string& font_path, FontSize size);
		Font(const std::string& font_path);
		~Font();

		template <FontSize Size>
		void Resize() { TTF_SetFontSize(font, (int)Size); }

		void RenderText(RenderSurface& surface, const std::string& str);
		void RenderText(RenderSurface& surface, const std::u16string& str);

		inline void ChangeForegroundColor(const Color& color) { font_color = color; }
		inline void ChangeForegroundColor(Byte r, Byte g, Byte b) { ChangeForegroundColor({ r, g, b, 0xFF }); }

		void Load() override;
		void Render() override;
		void Update() override;
	};
};