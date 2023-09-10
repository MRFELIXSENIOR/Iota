#pragma once

#include "TypeTraits.hpp"
#include "Conversion.hpp"

#include <string>
#include <functional>

#include <mono/metadata/loader.h>

namespace iota {
	namespace Mono {
		template <typename RT, typename... Args>
		struct InternalCall {
			using FunctionPtrType = RT(*)(Args...);

			template <typename T>
			using ArgType = ConvertMonoType<std::decay_t<T>>;

			template <typename T>
			using BoxedType = typename ArgType<T>::BoxedType;

			using ReturnType = ConvertMonoType<std::decay_t<RT>>;
			using CppType = typename ReturnType::CppType;

			RT(*function_ptr)(Args...);
			void* casted_function_ptr;
			std::string name;

			CppType Call(BoxedType<Args>... args) {
				RT&& value = function_ptr(ArgType<Args>::FromBoxedType(std::move(args))...);
				return ReturnType::ToMonoType(value);
			}

			CppType operator()(BoxedType<Args>... args) {
				return Call(std::forward<BoxedType<Args>>(args)...);
			}
		};

		template <typename... Args>
		struct InternalCall <void(Args...)> {
			using FunctionPtrType = void(*)(Args...);

			template <typename T>
			using ArgType = ConvertMonoType<std::decay_t<T>>;

			template <typename T>
			using BoxedType = typename ArgType<T>::BoxedType;

			void(*function_ptr)(Args...);
			void* casted_function_ptr;
			std::string name;

			void Call(BoxedType<Args>... args) {
				function_ptr(ArgType<Args>::FromBoxedType(std::move(args))...);
			}

			void operator()(BoxedType<Args>... args) {
				Call(std::forward<BoxedType<Args>>(args)...);
			}
		};

		template <typename Fn>
		struct InternalCall<std::function<Fn>> : public InternalCall<Fn> {};

		template <typename RT, typename... Args>
		InternalCall<RT(Args...)> WrapCall(const std::string& name, RT(*func_ptr)(Args...)) {
			InternalCall<RT(Args...)> call{};
			call.casted_function_ptr = reinterpret_cast<void*>(func_ptr);
			call.function_ptr = func_ptr;
			call.name = name;
			return call;
		}

		template <typename... Args>
		InternalCall<void(Args...)> WrapCall(const std::string& name, void(*func_ptr)(Args...)) {
			InternalCall<void(Args...)> call{};
			call.casted_function_ptr = reinterpret_cast<void*>(func_ptr);
			call.function_ptr = func_ptr;
			call.name = name;
			return call;
		}

		template <typename Signature>
		void AddInternalCall(InternalCall<Signature> call) {
			mono_add_internal_call(call.name.c_str(), call.casted_function_ptr);
		}

		template <typename... Signatures>
		void AddInternalCalls(InternalCall<Signatures>... calls) {
			(AddInternalCall(calls), ...);
		}
	}
}