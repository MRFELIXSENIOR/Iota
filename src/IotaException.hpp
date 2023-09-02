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

		struct ExceptionInfo {
			std::string message;
			std::string stack_trace;
			std::string class_name;
		};

		struct Exception : public RuntimeError {
		public:
			explicit Exception(MonoObject* mono_exc_object);

			const std::string& get_class_name() const noexcept;
			const std::string& get_message() const noexcept;
			const std::string& get_stack_trace() const noexcept;

		private:
			ExceptionInfo info;
		};

		struct ExceptionReference : public Exception {
		public:
			using Exception::Exception;

			void Raise();

			MonoException* get_mono_ptr() const;
			bool valid();

		private:
			MonoException* mono_exception;
			std::string name;
		};

		using ExceptionHandler = std::function<void(const ExceptionReference&)>;
	}
};
