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
		class Domain;

		enum class LogLevel {
			ERROR,
			CRITICAL,
			WARNING,
			MESSAGE,
			INFO,
			DEBUG,
		};

		class Exception {
		public:
			explicit Exception(const std::string& name, const std::string& msg);
			explicit Exception(MonoObject* exception);

			static void Raise(const std::string& name, const std::string& msg);
			static void Raise(MonoObject* exception);

			const char* what() const noexcept;
			const char* get_name() const noexcept;
			MonoException* get_mono_ptr() const;

		private:
			MonoException* mono_exception;
			std::string name;
			std::string message;
		};

		using ExceptionHandler = std::function<void(const Exception&)>;
	}
};
