#pragma once

#include "Type.hpp"

#include <mono/metadata/object.h>

namespace iota {
	namespace Mono {
		struct Object {
		private:
			MonoObject* self;
			Type type;

		public:
			explicit Object(MonoObject* mono_obj);
			explicit Object(const Type& klass);
			~Object();

			bool is_valid();
			MonoObject* get_mono_ptr() const;
			const Type& GetType() const;
		};
	}
}