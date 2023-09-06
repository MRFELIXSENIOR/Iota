#include "Property.hpp"
#include "TypeClass.hpp"
#include "Function.hpp"
#include "../IotaException.hpp"

using namespace iota;
using namespace Mono;

void Property::data() {
	Function get_fn = GetGetMethod();
	type = get_fn.GetReturnType();
	owner = TypeClass(mono_property_get_parent(self));
	name = mono_property_get_name(self);
}

Property::Property(MonoProperty* mono_property) : self(mono_property) {
	if (!self)
		throw RuntimeError("Cannot Create Mono Property Reference");
	data();
}

Property::Property(const TypeClass& klass, const std::string& name) : self(mono_class_get_property_from_name(klass.GetClassPointer(), name.c_str())) {
	if (!self)
		throw RuntimeError("Cannot Get Property of name: " + name + " on class: " + klass.GetName());
	data();
}

Function Property::GetGetMethod() const {
	return Function(mono_property_get_get_method(self));
}

Function Property::GetSetMethod() const {
	return Function(mono_property_get_set_method(self));
}


const TypeClass& Property::GetOwner() const { return owner; }
const TypeClass& Property::GetType() const { return type; }

bool Property::IsValid() { return self != nullptr; }

const std::string& Property::GetName() const { return name; }
MonoProperty* Property::GetDataPointer() const { return self; }
