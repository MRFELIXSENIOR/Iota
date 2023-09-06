#pragma once

#include "TypeClass.hpp"

#include <optional>

#include <mono/metadata/metadata.h>

namespace iota {
	namespace Mono {
		enum class AccessModifier {
			PUBLIC,
			PROTECTED,
			PRIVATE,
			INTERNAL,
			PROTECTED_INTERNAL
		};

		struct Function {
		private:
			void data();

		protected:
			MonoMethod* self = nullptr;
			MonoMethodSignature* sig = nullptr;

			std::string name;
			std::string fullname;

			friend struct Type;

		public:
			explicit Function(MonoMethod* mono_method);
			explicit Function(const TypeClass& klass, const std::string& fn_name, int argc);
			explicit Function(const TypeClass& klass, const std::string& fn_name_with_args);

			bool IsValid();
			bool IsStatic();
			bool IsVirtual();

			MonoMethod* GetDataPointer() const;
			MonoMethodSignature* GetSignaturePointer() const;
			const std::string& GetFullname() const;
			const std::string& GetName() const;

			AccessModifier GetAccessModifier() const;
			const TypeClass& GetReturnType() const;
			const std::vector<TypeClass>& GetArgTypes() const;
		};
	}
}