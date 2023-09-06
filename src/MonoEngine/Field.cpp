#include "Field.hpp"
#include "TypeClass.hpp"
#include "../IotaException.hpp"

#include <mono/metadata/attrdefs.h>
#include <mono/metadata/debug-helpers.h>

using namespace iota;
using namespace Mono;

void Field::data() {
	owner = TypeClass(mono_field_get_parent(self));
	type = TypeClass(mono_field_get_type(self));
	vtable = IsStatic()? mono_class_vtable(mono_get_root_domain(), owner.GetClassPointer()) : nullptr;
	name = mono_field_get_name(self);
	fullname = mono_field_full_name(self);
}

Field::Field(MonoClassField* mono_field) : self(mono_field) {
	if (!self)
		throw RuntimeError("Could Not Create Mono Field Reference");

	data();
}

Field::Field(const TypeClass& klass, const std::string& name) :
	self(mono_class_get_field_from_name(klass.GetClassPointer(), name.c_str()))
{
	if (!self)
		throw RuntimeError("Could Not Create Mono Field Reference: " + name + "in class: " + klass.GetName());

	data();
}

const TypeClass& Field::GetType() const { return type; }
const TypeClass& Field::GetOwnerClass() const { return owner; }
const std::string& Field::GetName() const { return name; }
const std::string& Field::GetFullname() const { return fullname; }

MonoClassField* Field::GetDataPointer() const { return self; }

bool Field::IsValueType() { return type.IsValueType(); }
bool Field::IsStatic() {
	unsigned int flags = mono_field_get_flags(self);
	return (flags & MONO_FIELD_ATTR_STATIC) != 0;
}
