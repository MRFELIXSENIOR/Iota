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

		struct Type final {
		private:
			MonoClass* self_class = nullptr;
			MonoType* self_type = nullptr;

			std::string name;
			std::string fullname;
			std::string _namespace;
			bool value_type;
			unsigned int size = 0;
			unsigned int alignof_ = 0;
			int rank;

			void data();

			friend struct Type;
			friend struct Object;
		public:
			Type() = default;
			explicit Type(MonoClass* mono_class);
			explicit Type(MonoType* mono_type);

			explicit Type(const std::string& name_space, const std::string& class_name, Context& ctx);
			explicit Type(const std::string& class_name, Context& ctx);
			~Type();

			Object CreateInstance();

			MonoClass* get_class_ptr() const;
			MonoType* get_type_ptr() const;

			bool is_valid();
			bool is_derived_from(const Type& suspect_base);
			bool is_derived();
			Type GetBaseClass();
			bool is_value_type();
			unsigned int get_sizeof();
			unsigned int get_alignof();

			const std::string& get_namespace() const;
			const std::string& get_fullname() const;
			const std::string& get_name() const;

			std::vector<Property> GetProperties() const;
			std::vector<Function> GetMethods() const;
			//std::vector<Field> GetFields() const;

			Property GetProperty(const std::string& name) const;
			//Field GetField(const std::string& name);

			Function GetFunction(const std::string& name, int arg_count) const;
			Function GetFunction(const std::string& name_with_args) const;
		};
	}
}