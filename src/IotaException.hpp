#pragma once

#include <string>
#include <functional>
#include <optional>
#include <stdexcept>
#include <exception>

namespace iota {
	class Error : public std::exception {
	public:
		using exception::exception;
	};

	class RuntimeError : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};
};
