#pragma once

#include "Object.hpp"
#include "String.hpp"
#include "../IotaException.hpp"

#include <string>

#include <mono/metadata/object.h>

namespace iota {
	namespace Mono {
		template <typename T>
		requires std::is_pod<T>::value
		auto GetMonoValue(T& t) { return std::addressof(t); }
		MonoObject* GetMonoValue(MonoObject* t);

		template <typename T>
		struct ConvertMonoType {
			using CppType = T;
			using UnboxedType = T;
			using BoxedType = MonoObject*;
			static UnboxedType ToMonoType(const T& obj) { return obj; }
			static CppType FromUnboxedType(const UnboxedType& obj) { return obj; }
			static CppType FromBoxedType(const BoxedType& obj) {
				Object o(obj);
				TypeClass type = o.GetClass();
				unsigned int mono_size = type.GetSize();
				unsigned int mono_align = type.GetAlign();
				unsigned int cpp_size = sizeof(CppType);
				unsigned int cpp_align = alignof(CppType);
				if (mono_size > cpp_size && mono_align > cpp_align)
					throw RuntimeError("Cannot Convert Mono Type To C++ Type with Different Type Layout");
				void* obj_ptr = mono_object_unbox(obj);
				return *reinterpret_cast<CppType*>(obj_ptr);
			}
		};

		template <>
		struct ConvertMonoType <Object> {
			using CppType = Object;
			using UnboxedType = MonoObject*;
			using BoxedType = MonoObject*;

			static UnboxedType ToMonoType(const CppType& obj) { return obj.GetDataPointer(); }
			static CppType FromUnboxedType(const UnboxedType& obj) { return Object(obj); }
			static CppType FromBoxedType(const BoxedType& obj) { return Object(obj); }
		};

		template <>
		struct ConvertMonoType <std::string> {
			using CppType = std::string;
			using UnboxedType = MonoObject*;
			using BoxedType = MonoObject*;
			static UnboxedType ToMonoType(const CppType& obj) {
				return String(obj).GetDataPointer();
			}

			static CppType FromUnboxedType(const UnboxedType& obj) {
				return String(Object(obj)).ToUTF8();
			}

			static CppType FromBoxedType(const BoxedType& obj) {
				return String(Object(obj)).ToUTF8();
			}
		};
	}
}
