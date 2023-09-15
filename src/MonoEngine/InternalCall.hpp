#pragma once

#include "TypeTraits.hpp"
#include "Conversion.hpp"

#include <string>
#include <functional>

#include <mono/metadata/loader.h>

namespace iota {
	namespace Mono {
		template <typename T>
		using ArgType = ConvertMonoType<std::decay_t<T>>;

		template <typename T>
		using BoxedArgType = typename ArgType<T>::BoxedType;

		template <typename Fn>
		struct InternalCall : public InternalCall<std::function<Fn>> {};

		template <typename RT, typename... Args>
		struct InternalCall<RT(Args...)> {
			void* wrapped_function_ptr;
			std::string name;
		};

		template <typename... Args>
		struct InternalCall <void(Args...)> {
			void* wrapped_function_ptr;
			std::string name;
		};

		template <typename Fn>
		struct InternalCall<std::function<Fn>> : public InternalCall<Fn> {};

		template <typename RT, typename... Args>
		InternalCall<RT(Args...)> WrapCall(const std::string& name, RT(*func_ptr)(Args...)) {
			using ReturnType = ConvertMonoType<std::decay_t<RT>>;
			using UnboxedRT = typename ReturnType::UnboxedType;

			InternalCall<RT(Args...)> call{};
			call.name = name;
			std::function<UnboxedRT(BoxedArgType<Args>...)> wrapped = [&](BoxedArgType<Args>... args) -> UnboxedRT {
				RT&& value = func_ptr(ArgType<Args>::FromBoxedType(std::move(args))...);
				return ReturnType::ToMonoType(value);
			};

			call.wrapped_function_ptr = reinterpret_cast<void*>(wrapped.target<UnboxedRT(*)(BoxedArgType<Args>...)>());
			return call;
		}

		template <typename RT, typename... Args>
		auto WrapCall(const std::string& name, std::function<RT(Args...)> func) {
			return WrapCall(name, func.target<RT(*)(Args...)>());
		}

		template <typename... Args>
		InternalCall<void(Args...)> WrapCall(const std::string& name, void(*func_ptr)(Args...)) {
			InternalCall<void(Args...)> call{};
			call.name = name;
			std::function<void(BoxedArgType<Args>...)> wrapped = [&](BoxedArgType<Args>... args) -> void {
				func_ptr(ArgType<Args>::FromBoxedType(std::move(args))...);
			};

			call.wrapped_function_ptr = reinterpret_cast<void*>(wrapped.target<void(*)(BoxedArgType<Args>...)>());
			return call;
		}

		template <typename... Args>
		auto WrapCall(const std::string& name, std::function<void(Args...)> func) {
			return WrapCall(name, func.target<void(*)(Args...)>());
		}

		template <typename Signature>
		void AddInternalCall(InternalCall<Signature> call) {
			mono_add_internal_call(call.name.c_str(), call.wrapped_function_ptr);
		}

		template <typename... Signatures>
		void AddInternalCalls(InternalCall<Signatures>... calls) {
			(AddInternalCall(calls), ...);
		}
	}
}