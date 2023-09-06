#pragma once

#include "IotaBehavior.hpp"
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

	class Font : protected GameBehavior {
	private:
		std::unordered_map<RenderSurface*, std::string> str_container;
		Color color;
		TTF_Font* font;

	public:
		Font(const std::string& font_path, FontSize size);
		Font(const std::string& font_path);
		~Font();

		void Resize(FontSize size);
		void RenderText(RenderSurface& surface, const std::string& str);

		void Load() override;
		void Render() override;
		void Update() override;
	};
};