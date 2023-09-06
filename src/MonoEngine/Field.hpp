#pragma once

#include "TypeClass.hpp"
#include "Object.hpp"
#include "Conversion.hpp"

#include <string>
#include <type_traits>

#include <mono/metadata/object.h>

namespace iota {
	namespace Mono {
		struct Object;

		struct Field {
		private:
			void data();

		protected:
			MonoClassField* self = nullptr;
			MonoVTable* vtable = nullptr;

			std::string name;
			std::string fullname;

			TypeClass owner;
			TypeClass type;

			template <typename T>
			void GetImpl(const Object* obj, T& output) {
				T out{};
				MonoObject* mobj_val = nullptr;
				void* out_ptr = reinterpret_cast<void*>(&out);
				if (!IsValueType())
					out_ptr = &mobj_val;

				if (obj != nullptr) {
					MonoObject* obj_ptr = obj->GetPointer();
					mono_field_get_value(obj_ptr, self, out_ptr);
				}
				else {
					mono_field_static_get_value(vtable, self, out_ptr);
				}

				if (!IsValueType())
					out = ConvertMonoType<T>::FromBoxedType(mobj_val);

				output = out;
			}

			template <typename T>
			void SetImpl(const Object* obj, const T& value) {
				auto mono_val = ConvertMonoType<T>::ToMonoType(value);
				auto set_val = GetMonoValue(mono_val);

				if (obj) {
					MonoObject* obj_ptr = obj->GetPointer();
					mono_field_set_value(obj_ptr, self, set_val);
				}
				else {
					mono_field_static_set_value(vtable, self, set_val);
				}
			}

		public:
			explicit Field(MonoClassField* mono_field);
			explicit Field(const TypeClass& klass, const std::string& name);

			template <typename T>
			requires std::is_assignable<T&, T&&>::value
			void Get(const Object& object, T& output) const { GetImpl(&object, std::forward<T>(output)); }

			template <typename T>
			requires std::is_assignable<T&, T&&>::value
			void Get(T& output) const { GetImpl(nullptr, std::forward<T>(output)); }

			template <typename T>
			void Set(const Object& object, T& value) { SetImpl(&object, value); }

			template <typename T>
			void Set(const T& value) const { SetImpl(nullptr, value); }

			const TypeClass& GetType() const;
			const TypeClass& GetOwnerClass() const;
			const std::string& GetName() const;
			const std::string& GetFullname() const;

			MonoClassField* GetDataPointer() const;

			bool IsStatic();
			bool IsValueType();
		};
	}
}