#pragma once

#include "Function.hpp"
#include "TypeTraits.hpp"
#include "Conversion.hpp"

#include <mono/metadata/object.h>

namespace iota {
	namespace Mono {
		template <IsInvokable T>
		struct Invoker;

		template <typename... Args>
		struct Invoker <void(Args...)> : public Function {
		public:
			static Invoker& MakeInvoker(const Function& fn) {
				if (!HasCompatibleSignature<void(Args...)>(fn))
					throw RuntimeError("Incompatible Arguments");

				return Invoker<void(Args...)>(fn);
			}

			void Invoke(const Object* obj, Args... args) {
				MonoObject* mobj = nullptr;
				if (obj) {
					mobj = obj->get_mono_ptr();
					self = mono_object_get_virtual_method(mobj, self);
				}

				std::tuple<Args...> tuple =
					std::make_tuple(ConvertMonoType<std::decay_t<Args>>::ToMonoType(std::forward<Args>(args))...);

				auto caller = [&](auto... args) {
					std::vector<void*> argsv = { std::addressof(args)... };
					MonoObject* exc = nullptr;
					mono_runtime_invoke(self, mobj, argsv.data(), &exc);
					//if (exc)
						//throw FunctionInvokeException(exc)
					};

				std::apply(caller, tuple);
			}
		private:
			using Function::Function;
		};

		template <typename RT, typename... Args>
		struct Invoker <RT(Args...)> : public Function {
			static Invoker& MakeInvoker(const Function& fn) {
				if (!HasCompatibleSignature<RT(Args...)>(fn))
					throw RuntimeError("Incompatible Signature")
			}

			RT Invoke(const Function& fn, const Object* obj, Args... args) {
				MonoObject* mobj = nullptr;
				if (obj) {
					mobj = obj->get_mono_ptr();
					fn.self = mono_object_get_virtual_method(mobj, fn.self);
				}

				std::tuple<Args...> tuple =
					std::make_tuple(ConvertMonoType<std::decay_t<Args>>::ToMonoType(std::forward<Args>(args))...);

				auto caller = [&](auto... args) {
					std::vector<void*> argsv = { std::addressof(args)... };
					MonoObject* exc = nullptr;
					MonoObject* ret_val = mono_runtime_invoke(fn.self, mobj, argsv.data(), &exc);
					//if (exc)
						//throw FunctionInvokeException(exc);

					return ret_val;
					};

				auto result = std::apply(caller, tuple);
				return ConvertMonoType<std::decay_t<RT>>::FromBoxedType(std::move(result));
			}

		private:
			using Function::Function;
		};
	}
}