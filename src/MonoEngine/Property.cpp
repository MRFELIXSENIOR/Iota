#include "Property.hpp"
#include "Type.hpp"
#include "Function.hpp"
#include "../IotaException.hpp"

using namespace iota;
using namespace Mono;

void Property::data() {
	Function get_fn = GetGetMethod();
	type = get_fn.GetReturnType();
	owner = Type(mono_property_get_parent(self));
	name = mono_property_get_name(self);
}

Property::Property(MonoProperty* mono_property) : self(mono_property) {
	if (!self)
		throw RuntimeError("Cannot Create Mono Property Reference");
	data();
}

Property::Property(const Type& klass, const std::string& name) : self(mono_class_get_property_from_name(klass.get_class_ptr(), name_.c_str())) {
	if (!self)
		throw RuntimeError("Cannot Get Property of name: " + name + " on class: " + klass.get_name());
	data();
}

Function Property::GetGetMethod() const {
	return Function(mono_property_get_get_method(self));
}

Function Property::GetSetMethod() const {
	return Function(mono_property_get_set_method(self));
}


const Type& Property::GetOwner() const { return owner; }
const Type& Property::GetType() const { return type; }

bool Property::is_valid() { return self != nullptr; }

const std::string& Property::get_name() const { return name; }
MonoProperty* Property::get_mono_ptr() const { return self; }
