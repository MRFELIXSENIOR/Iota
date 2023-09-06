#pragma once

#include <string>
#include <functional>
#include <optional>
#include <stdexcept>
#include <exception>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/exception.h>

namespace iota {
	class Error : public std::exception {
	public:
		using exception::exception;
	};

	class RuntimeError : public std::runtime_error {
	public:
		using runtime_error::runtime_error;
	};

	namespace Mono {

		struct Exception {
		public:
			explicit Exception(MonoObject* mono_exc_object);

			const std::string& GetClassName() const noexcept;
			const std::string& GetMessage() const noexcept;
			const std::string& GetStacktrace() const noexcept;

		private:
			std::string message;
			std::string stack_trace;
			std::string class_name;
		};

		using ExceptionHandler = std::function<void(const Exception&)>;
		void DefaultExceptionHandler(const Exception&);
	}
};
