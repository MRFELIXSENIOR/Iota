#pragma once

#include "IotaBehavior.hpp"

#include <string>
#include <vector>
#include <SDL_ttf.h>

namespace iota {
	enum class FontSize {
		SMALL = 12,
		MEDIUM = 18,
		LARGE = 24
	};

	class Font : protected GameBehavior {
	private:
		std::vector<std::string> str_container;
		Color color;
		TTF_Font* font;

	public:
		Font(const std::string& font_path, FontSize size);
		Font(const std::string& font_path);

		~Font();

		void Resize(FontSize size);
		void RenderText(const std::string& str);

		void SetColor(Color color);

		void Load() override;
		void Render() override;
		void Update() override;
	};
};