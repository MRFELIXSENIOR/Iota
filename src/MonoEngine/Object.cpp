#include "Object.hpp"
#include "../IotaException.hpp"

#include <mono/metadata/debug-helpers.h>

using namespace iota;
using namespace Mono;

Object::Object(MonoObject* mono_obj): self(mono_obj) {
	if (self == nullptr)
		throw RuntimeError("Cannot Create Object Reference");

	klass = TypeClass(mono_object_get_class(self));
}

Object::Object(const TypeClass& cl_ref): self (mono_object_new(mono_get_root_domain(), cl_ref.GetClassPointer())) {
	mono_runtime_object_init(self);
	if (self == nullptr)
		throw RuntimeError("Cannot Create Object of class: " + cl_ref.GetName());

	klass = cl_ref;
}

bool Object::IsValid() { return self != nullptr; }

MonoObject* Object::GetDataPointer() const { return self; }
const TypeClass& Object::GetClass() const { return klass; }
