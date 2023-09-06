#pragma once

#include "TypeClass.hpp"

#include <mono/metadata/object.h>

namespace iota {
	namespace Mono {
		struct Object {
		private:
			MonoObject* self;
			TypeClass klass;

		public:
			explicit Object(MonoObject* mono_obj);
			explicit Object(const TypeClass& klass);

			bool IsValid();

			MonoObject* GetDataPointer() const;
			const TypeClass& GetClass() const;
		};
	}
}