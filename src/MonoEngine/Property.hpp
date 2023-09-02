#pragma once

#include "FunctionInvoker.hpp"

#include <string>

#include <mono/metadata/class.h>

namespace iota {
	namespace Mono {
		struct Type;
		struct Function;

		struct Property final {
		private:
			MonoProperty* self;
			Type type;
			Type owner;
			std::string name;

			void data();

		public:
			explicit Property(MonoProperty* mono_property);
			explicit Property(const Type& klass, const std::string& name);

			Function GetGetMethod() const;
			Function GetSetMethod() const;

			template <typename T>
			T Get() const {
				return GetGetMethod().Invoke<T>();
			}

			template <typename T>
			void Set(const T& value) const {
				GetSetMethod().Invoke<void(const T&)>(value);
			}

			const Type& GetOwner() const;
			const Type& GetType() const;

			bool is_valid();

			const std::string& get_name() const;
			MonoProperty* get_mono_ptr() const;
		};
	}
}