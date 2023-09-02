#pragma once

#include <optional>

#include <mono/metadata/metadata.h>

namespace iota {
	namespace Mono {
		struct Type;

		enum class AccessModifier {
			PUBLIC,
			PROTECTED,
			PRIVATE,
			INTERNAL,
			PROTECTED_INTERNAL
		};

		struct Function {
		private:
			MonoMethod* self = nullptr;
			MonoMethodSignature* sig = nullptr;

			std::string name;
			std::string fullname;

			friend struct Type;
			void data();

		public:
			explicit Function(MonoMethod* mono_method);
			explicit Function(const Type& klass, const std::string& fn_name, int argc);
			explicit Function(const Type& klass, const std::string& fn_name_with_args);
			~Function();

			bool is_valid();
			bool is_static();
			bool is_virtual();

			MonoMethod* get_mono_ptr() const;
			MonoMethodSignature* get_signature_ptr() const;
			const std::string& get_fullname() const;
			const std::string& get_name() const;

			AccessModifier GetAccessModifier() const;
			const Type& GetReturnType() const;
			const std::vector<Type>& GetArgTypes() const;
		};
	}
}