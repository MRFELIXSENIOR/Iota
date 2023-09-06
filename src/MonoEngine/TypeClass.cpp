#include "TypeClass.hpp"
#include "Function.hpp"
#include "Property.hpp"
#include "Field.hpp"
#include "Object.hpp"
#include "Context.hpp"
#include "../IotaException.hpp"

#include <mono/metadata/debug-helpers.h>

using namespace iota;
using namespace Mono;

void TypeClass::data() {
	self_type = mono_class_get_type(self_class);
	name = mono_class_get_name(self_class);
	_namespace = mono_class_get_namespace(self_class);
	fullname = _namespace.empty() ? name : _namespace + "." + name;
	value_type = mono_class_is_valuetype(self_class);
	size = (unsigned int)(mono_class_value_size(self_class, &alignof_));
}

TypeClass::TypeClass(MonoClass* mono_class): self_class(mono_class) {
	if (!self_class)
		throw Error("Cannot Create Mono Type Reference");
	data();
}

TypeClass::TypeClass(MonoType* mono_type): self_class(mono_class_from_mono_type(mono_type)) {
	if (!self_class)
		throw RuntimeError("Cannot Create Mono Type Reference");
	data();
}

TypeClass::TypeClass(const std::string& name_space, const std::string& class_name, const Context& ctx): self_class(mono_class_from_name(ctx.GetImage(), name_space.c_str(), class_name.c_str())) {
	if (!self_class)
		throw RuntimeError("Cannot Create Mono Type Reference with name: " + class_name);
	data();
}

TypeClass::TypeClass (const std::string& class_name, const Context& ctx) : TypeClass("", class_name, ctx) {}

Object TypeClass::CreateInstance() const { return Object(*this); }

bool TypeClass::IsValid() { return self_class != nullptr; }
bool TypeClass::IsDerivedFrom(const TypeClass& sus_base) { return mono_class_is_subclass_of(self_class, sus_base.GetClassPointer(), false) != 0; }
bool TypeClass::IsDerived() { return mono_class_get_parent(self_class) != nullptr; }
bool TypeClass::IsValueType() { return value_type; }

TypeClass TypeClass::GetBaseClass() const { return TypeClass(mono_class_get_parent(self_class)); }

unsigned int TypeClass::GetSize() const { return size; }
unsigned int TypeClass::GetAlign() const { return alignof_; }

std::vector<Property> TypeClass::GetProperties() const {
	void* iter = nullptr;
	MonoProperty* prop = mono_class_get_properties(self_class, &iter);
	std::vector<Property> vec;
	while (prop != nullptr) {
		vec.emplace_back(prop);
		prop = mono_class_get_properties(self_class, &iter);
	}

	return vec;
}

std::vector<Function> TypeClass::GetMethods() const {
	void* iter = nullptr;
	MonoMethod* method = mono_class_get_methods(self_class, &iter);
	std::vector<Function> vec;

	while (method != nullptr) {
		vec.emplace_back(method);
		method = mono_class_get_methods(self_class, &iter);
	}

	return vec;
}

std::vector<Field> TypeClass::GetFields() const {
	void* iter = nullptr;
	MonoClassField* field = mono_class_get_fields(self_class, &iter);
	std::vector<Field> vec;

	while (field != nullptr) {
		vec.emplace_back(field);
		field = mono_class_get_fields(self_class, &iter);
	}

	return vec;
}

Property TypeClass::GetProperty(const std::string& name) const {
	return Property(*this, name);
}

Function TypeClass::GetFunction(const std::string& name, int arg_count) const {
	return Function(*this, name, arg_count);
}

Function TypeClass::GetFunction(const std::string& name_with_args) const {
	return Function(*this, name_with_args);
}

Field TypeClass::GetField(const std::string& name) const {
	return Field(*this, name);
}

MonoClass* TypeClass::GetClassPointer() const { return self_class; }
MonoType* TypeClass::GetTypePointer() const { return self_type; }

const std::string& TypeClass::GetNamespace() const { return _namespace; }
const std::string& TypeClass::GetFullname() const { return fullname; }
const std::string& TypeClass::GetName() const { return name; }

