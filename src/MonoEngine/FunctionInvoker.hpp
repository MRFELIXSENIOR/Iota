#pragma once

#include "Function.hpp"
#include "TypeTraits.hpp"
#include "Conversion.hpp"

#include <string>
#include <optional>

#include <mono/metadata/object.h>

namespace iota {
	namespace Mono {
		template <typename T>
		void TypeCheckWrapper(const std::string& fullname, bool& output) {
			output &= IsTypeFullnameCompatible<T>(fullname);
		}

		template <typename TupleLike, size_t... Index>
		bool TupleTypeCheckImpl(std::index_sequence<Index...>, const std::vector<TypeClass>& cl_vec) {
			bool result = true;
			(TypeCheckWrapper<std::tuple_element_t<Index, TupleLike>>(cl_vec[Index].GetFullname(), result), ...);
			return result;
		}

		template <typename TupleLike>
		bool TupleTypeCheck(const std::vector<TypeClass>& cl_vec) {
			return TupleTypeCheckImpl<TupleLike>(
				std::make_index_sequence<std::tuple_size_v<TupleLike>>{},
				cl_vec
			);
		}

		template <typename Signature>
		bool HasCompatibleSignature(const Function& function) {
			using ReturnType = typename FunctionTraits<Signature>::ReturnType;
			using ArgTypes = typename FunctionTraits<Signature>::ArgTypesDecayed;
			constexpr size_t ArgCount = FunctionTraits<Signature>::ArgCount;

			const TypeClass& expected_rt = function.GetReturnType();
			const std::vector<TypeClass>& expected_arg_types = function.GetArgTypes();

			if (
				!(ArgCount == expected_arg_types.size()) &&
				!IsTypeFullnameCompatible<ReturnType>(expected_rt.GetFullname())
				)
				return false;

			bool result = TupleTypeCheck<ArgTypes>(expected_arg_types);
			return result;
		}

		struct InvokeResult {
			std::optional<Exception> maybe_exc;
			bool success;
		};

		template <IsInvokable T>
		struct Invoker;

		template <typename... Args>
		struct Invoker <void(Args...)> : public Function {
		public:
			Invoker(const Function& fn): Function(fn) {
				if (!HasCompatibleSignature<void(Args...)>(fn))
					throw RuntimeError("Incompatible Arguments");
			}

			void operator()(const Object& obj, const ExceptionHandler& exc_handler, Args... args) {
				Invoke(&obj, exc_handler, std::forward<Args>(args)...);
			}

			void operator()(const Object& obj, Args... args) {
				Invoke(&obj, DefaultExceptionHandler, std::forward<Args>(args)...);
			}

			void operator()(const ExceptionHandler& exc_handler, Args... args) {
				Invoke(nullptr, exc_handler, std::forward<Args>(args)...);
			}

			void operator()(Args&&... args) {
				Invoke(nullptr, DefaultExceptionHandler, std::forward<Args>(args)...);
			}

			InvokeResult GetResult() const { return result; }
		private:
			InvokeResult result;

			void Invoke(const Object* obj, const ExceptionHandler& exc_handler, Args... args) {
				MonoObject* mobj = nullptr;
				if (obj) {
					mobj = obj->GetPointer();
					self = mono_object_get_virtual_method(mobj, self);
				}

				auto tuple =
					std::make_tuple(ConvertMonoType<std::decay_t<Args>>::ToMonoType(std::forward<Args>(args))...);

				auto caller = [&](auto... inv_args) {
					std::vector<void*> argsv = { std::addressof(inv_args)... };
					MonoObject* exc = nullptr;
					mono_runtime_invoke(self, mobj, argsv.data(), &exc);
					if (exc) {
						Exception exc_obj(exc);
						exc_handler(exc_obj);
						result.maybe_exc = std::make_optional(exc_obj);
						result.success = false;
					}

					result.maybe_exc = std::nullopt;
					result.success = true;
				};

				std::apply(caller, tuple);
			}
		};

		template <typename RT, typename... Args>
		struct Invoker <RT(Args...)> : public Function {
			Invoker(const Function& fn): Function(fn) {
				if (!HasCompatibleSignature<RT(Args...)>(fn))
					throw RuntimeError("Incompatible Signature");
			}

			RT operator()(const Object& obj, const ExceptionHandler& exc_handler, Args... args) {
				return Invoke(&obj, exc_handler, std::forward<Args>(args)...);
			}

			RT operator()(const Object& obj, Args... args) {
				return Invoke(&obj, DefaultExceptionHandler, std::forward<Args>(args)...);
			}

			RT operator()(const ExceptionHandler& exc_handler, Args... args) {
				return Invoke(nullptr, exc_handler, std::forward<Args>(args)...);
			}

			RT operator()(Args... args) {
				return Invoke(nullptr, DefaultExceptionHandler, std::forward<Args>(args)...);
			}

			InvokeResult GetResult() const { return result; }
		private:
			InvokeResult result;
			RT Invoke(const Object* obj, const ExceptionHandler& exc_handler, Args... args) {
				MonoObject* mobj = nullptr;
				if (obj) {
					mobj = obj->GetPointer();
					self = mono_object_get_virtual_method(mobj, self);
				}

				auto tuple =
					std::make_tuple(ConvertMonoType<std::decay_t<Args>>::ToMonoType(std::forward<Args>(args))...);

				auto caller = [&](auto... inv_args) {
					std::vector<void*> argsv = { std::addressof(inv_args)... };
					MonoObject* exc = nullptr;
					MonoObject* ret_val = mono_runtime_invoke(self, mobj, argsv.data(), &exc);
					if (exc) {
						Exception exc_obj(exc);
						exc_handler(exc_obj);
						result.maybe_exc = std::make_optional(exc_obj);
						result.success = false;
					}

					result.maybe_exc = std::nullopt;
					result.success = true;

					return ret_val;
				};

				auto result = std::apply(caller, tuple);
				return ConvertMonoType<std::decay_t<RT>>::FromBoxedType(std::move(result));
			}
		};
	}
}