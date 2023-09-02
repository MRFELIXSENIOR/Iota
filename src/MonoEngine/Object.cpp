#include "Object.hpp"
#include "../IotaException.hpp"

#include <mono/metadata/debug-helpers.h>

using namespace iota;
using namespace Mono;

Object::Object(MonoObject* mono_obj): self(mono_obj) {
	if (!self)
		throw RuntimeError("Cannot Create Object Reference");
}

Object::Object(const Type& cl_ref): self (mono_object_new(mono_get_root_domain(), cl_ref.get_class_ptr())) {
	mono_runtime_object_init(self);
	if (!self)
		throw RuntimeError("Cannot Create Object of class: " + cl_ref.get_name());
}

MonoObject* Object::get_mono_ptr() const { return self; }
bool Object::is_valid() { return self != nullptr; }
const Type& Object::GetType() const { return type; }
