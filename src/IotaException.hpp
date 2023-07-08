#pragma once

#include <string>
#include <exception>

namespace iota {
	namespace Application {
		void Error(std::string_view msg);
		void Error(std::string_view title, std::string_view msg);

		void Panic(std::string_view msg);
		void Panic(std::string_view title, std::string_view msg);
	};
};
