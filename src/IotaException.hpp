#pragma once

#include <string>
#include <exception>

namespace iota {
	namespace Application {
		void Error(const std::string& msg);
		void Error(const std::string& title, const std::string& msg);

		void Panic(const std::string& msg);
		void Panic(const std::string& title, const std::string& msg);
	};
};
