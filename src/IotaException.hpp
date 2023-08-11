#pragma once

#include <string>
#include <stdexcept>
#include <exception>

namespace iota {
	enum class ErrorType {
		ERROR,
		RUNTIME_ERROR
	};

	namespace Application {
		void Throw(ErrorType type, const std::string& msg);
		void Throw(ErrorType type, const std::string& title, const std::string& msg);

		class Error : public std::exception {
		public:
			Error(const std::string& msg) : std::exception(msg.c_str()) {}
		};

		class RuntimeError : public std::runtime_error {
		public:
			RuntimeError(const std::string& msg) : std::runtime_error(msg) {}
		};
	};
};
