#pragma once

#include <string>
#include <stdexcept>
#include <exception>

namespace iota {
	namespace Application {
		class Error : public std::exception {
		public:
			using exception::exception;
		};

		class RuntimeError : public std::runtime_error {
		public:
			using runtime_error::runtime_error;
		};

		void SetDefaultExceptionHandler();
	};
};
