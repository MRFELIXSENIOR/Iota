#pragma once

#include "FunctionInvoker.hpp"

#include <string>
#include <type_traits>

#include <mono/metadata/class.h>

namespace iota {
	namespace Mono {
		struct TypeClass;
		struct Function;

		struct Property final {
		private:
			MonoProperty* self;
			TypeClass type;
			TypeClass owner;
			std::string name;

			void data();

		public:
			explicit Property(MonoProperty* mono_property);
			explicit Property(const TypeClass& klass, const std::string& name);

			Function GetGetMethod() const;
			Function GetSetMethod() const;

			template <typename T>
			requires std::is_assignable<T&, T&&>::value
			void Get(T& output) const {
				Invoker<T()> invoker(GetGetMethod());
				output = invoker();
			}

			template <typename T>
			void Set(const T& value) const {
				Invoker<void(const T&)> invoker(GetSetMethod());
				invoker(value);
			}

			const TypeClass& GetOwner() const;
			const TypeClass& GetType() const;

			bool IsValid();

			const std::string& GetName() const;
			MonoProperty* GetDataPointer() const;
		};
	}
}