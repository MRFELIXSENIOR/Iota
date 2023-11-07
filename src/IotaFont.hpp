#pragma once

#include "IotaGameInstance.hpp"
#include "IotaBasic.hpp"
#include "IotaDef.hpp"

#include <string>
#include <unordered_map>
#include <variant>

#include <SDL_ttf.h>

namespace iota {
	class RenderSurface;

	enum class FontSize {
		SMALL = 12,
		MEDIUM = 18,
		LARGE = 24
	};

	class IOTA_API Font : ActorInterface {
	private:
		struct UTF8Text {
			std::string text;
			Color bg, fg;
			RenderSurface* surface;
		};

		struct UTF16Text { 
			std::u16string text;
			Color bg, fg;
			RenderSurface* surface;
		};

		std::vector<UTF8Text> str8_container;
		std::vector<UTF16Text> str16_container;

		TTF_Font* font;

	public:
		using ActorInterface::ActorInterface;
		Font(const std::string& font_path, FontSize size);
		Font(const std::string& font_path);
		~Font();

		template <FontSize Size>
		void Resize() { TTF_SetFontSize(font, (int)Size); }

		void RenderText(RenderSurface& surface, const std::string& str, const Color& background, const Color& foreground);
		void RenderText(RenderSurface& surface, const std::string& str, const Color& foreground);
		void RenderText(RenderSurface& surface, const std::u16string& str, const Color& background, const Color& foreground);
		void RenderText(RenderSurface& surface, const std::u16string& str, const Color& foreground);

	protected:
		void Load();
		void Render();
		void Update(float dt);
	};
}