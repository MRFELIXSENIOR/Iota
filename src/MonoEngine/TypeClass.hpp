#pragma once

#include <string>
#include <vector>

#include <mono/metadata/metadata.h>

namespace iota {
	namespace Mono {
		struct Object;
		struct Context;
		struct Function;
		struct Property;
		struct Field;

		struct TypeClass final {
		private:
			MonoClass* self_class = nullptr;
			MonoType* self_type = nullptr;

			std::string name;
			std::string fullname;
			std::string _namespace;
			bool value_type = true;
			unsigned int size = 0;
			unsigned int alignof_ = 0;
			int rank = 0;

			void data();

			friend struct Object;
		public:
			TypeClass() = default;
			explicit TypeClass(MonoClass* mono_class);
			explicit TypeClass(MonoType* mono_type);

			explicit TypeClass(const std::string& name_space, const std::string& class_name, const Context& ctx);
			explicit TypeClass(const std::string& class_name, const Context& ctx);

			Object CreateInstance() const;

			MonoClass* GetClassPointer() const;
			MonoType* GetTypePointer() const;

			bool IsValid();
			bool IsDerivedFrom(const TypeClass& suspect_base);
			bool IsDerived();
			bool IsValueType();

			unsigned int GetSize() const;
			unsigned int GetAlign() const;

			TypeClass GetBaseClass() const;

			const std::string& GetNamespace() const;
			const std::string& GetFullname() const;
			const std::string& GetName() const;

			std::vector<Property> GetProperties() const;
			std::vector<Function> GetMethods() const;
			std::vector<Field> GetFields() const;

			Property GetProperty(const std::string& name) const;
			Function GetFunction(const std::string& name, int arg_count) const;
			Function GetFunction(const std::string& name_with_args) const;
			Field GetField(const std::string& name) const;
		};
	}
}