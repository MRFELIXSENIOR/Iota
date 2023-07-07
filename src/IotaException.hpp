#pragma once

#include <string>
#include <exception>

namespace IotaEngine {
	namespace Application {
		class RuntimeException : public std::runtime_error {
		public:
			explicit RuntimeException(const std::string& str) : std::runtime_error(str) {}
		};

		class Exception : public std::exception {
		public:
			explicit Exception(const std::string& str) : std::exception(str) {}
		};
	};
};
