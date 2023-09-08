#pragma once

#include "TypeTraits.hpp"
#include "Conversion.hpp"

#include <string>
#include <functional>

#include <mono/metadata/loader.h>

namespace iota {
	namespace Mono {
		template <typename Fn>
		void AddInternalCall(const std::string& name, Fn&& fn) {
			mono_add_internal_call(name.c_str(), reinterpret_cast<void*>(fn));
		}

		template <typename Fn>
		struct InternalCall {
			Fn function_ptr;
			std::string name;
		};

		template <typename RT, typename... Args>
		struct InternalCall<RT(Args...)> {
			RT(*function_ptr)(Args...);
			std::string name;
		};

		template <typename OC, typename RT, typename... Args>
		struct InternalCall<RT(OC::*)(Args...)> {
			RT(OC::*function_ptr)(Args...);
			std::string name;
		};

		template <typename Fn, Fn& func>
		struct InternalCallWrapper;

		template <typename... Args, void(&func)(Args...)>
		struct InternalCallWrapper<void(Args...), func> {
			template <typename T>
			using ArgType = ConvertMonoType<std::decay_t<T>>;

			template <typename T>
			using BoxedType = typename ArgType<T>::BoxedType;

			static void Call(BoxedType<Args>... args) {
				func(ArgType<Args>::FromBoxedType(std::move(args))...);
			}
		};

		template <typename RT, typename... Args, RT(&func)(Args...)>
		struct InternalCallWrapper<RT(Args...), func> {
			using ReturnType = ConvertMonoType<std::decay_t<RT>>;
			using CppType = typename ReturnType::CppType;

			template <typename T>
			using ArgType = ConvertMonoType<std::decay_t<T>>;

			template <typename T>
			using BoxedType = typename ArgType<T>::BoxedType;

			static CppType Call(BoxedType<Args>... args) {
				return ReturnType::ToMonoType(func(ArgType<Args>::FromBoxedType(std::move(args))...));
			}
		};

		template <typename Fn, Fn func>
		struct MemberFunctionWrapper;

		template <typename OC, typename... Args, void(OC::*func)(Args...)>
		struct MemberFunctionWrapper <void(OC::*)(Args...), func> {
			template <typename T>
			using ArgType = ConvertMonoType<std::decay_t<T>>;

			template <typename T>
			using BoxedType = typename ArgType<T>::BoxedType;

			static void Call(OC* obj, BoxedType<Args>... args) {
				(obj.*func)(ArgType<Args>::FromBoxedType(std::move(args))...);
			}
		};

		template <typename OC, typename RT, typename... Args, RT(OC::*func)(Args...)>
		struct MemberFunctionWrapper<RT(OC::*)(Args...), func> {
			using ReturnType = ConvertMonoType<std::decay_t<RT>>;
			using CppType = typename ReturnType::CppType;

			template <typename T>
			using ArgType = ConvertMonoType<std::decay_t<T>>;

			template <typename T>
			using BoxedType = typename ArgType<T>::BoxedType;

			static CppType Call(OC* obj, BoxedType<Args>... args) {
				return ReturnType::ToMonoType((obj.*func)(ArgType<Args>::FromBoxedType(std::move(args))...));
			}
		};

		template <typename FnType>
		InternalCall<FnType> MakeInternalCall(const std::string& name, FnType&& func) {
			InternalCall<FnType> internal_call{};
			internal_call.function_ptr = &InternalCallWrapper<FnType, func>::Call;
			internal_call.name = name;
			return internal_call;
		}

		template <typename OC, typename RT, typename... Args>
		InternalCall<RT(OC::*)(Args...)> MakeInternalCall(const std::string& name, std::function<RT(OC*, Args...)> func) {
			InternalCall<RT(OC::*)(Args...)> internal_call{};
			internal_call.function_ptr = &MemberFunctionWrapper<RT(OC::*)(Args...), func.target<RT(OC*, Args...)>()>::Call;
			internal_call.name = name;
			return internal_call;
		}

		template <typename... Signatures>
		void AddInternalCalls(InternalCall<Signatures>... calls) {
			(AddInternalCall(calls.name, calls.function_ptr), ...);
		}
	}
}