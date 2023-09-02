#include "Type.hpp"
#include "Function.hpp"
#include "Property.hpp"
#include "Object.hpp"
#include "../IotaException.hpp"

#include <mono/metadata/debug-helpers.h>

using namespace iota;
using namespace Mono;

void Type::data() {
	self_type = mono_class_get_type(self_class);
	name = mono_class_get_name(self_class);
	_namespace = mono_class_get_namespace(self_class);
	fullname = _namespace.empty() ? name : _namespace + "." + name;
	value_type = mono_class_is_valuetype(self_class);
	size = (unsigned int)(mono_class_value_size(self_class, &alignof_));
}

Type::Type(MonoClass* mono_class): self_class(mono_class) {
	if (!self_class)
		throw Error("Cannot Create Mono Type Reference");
	data();
}

Type::Type(MonoType* mono_type): self_class(mono_type_get_class(mono_type)) {
	if (!self_class)
		throw RuntimeError("Cannot Create Mono Type Reference");
	data();
}

Type::Type(const std::string& name_space, const std::string& class_name, Context& ctx): self_class(mono_class_from_name(ctx.img, name_space.c_str(), class_name.c_str())) {
	if (!self_class)
		throw RuntimeError("Cannot Create Mono Type Reference with name: " + class_name);
	data();
}

Type::Type(const std::string& class_name, Context& ctx) : Type("", class_name, ctx) {}
Type::~Type() {}

Object Type::CreateInstance() { return Object(*this); }

bool Type::is_valid() { return self_class != nullptr; }
bool Type::is_derived_from(const Type& sus_base) { return mono_class_is_subclass_of(self_class, sus_base.get_class_ptr(), false) != 0; }
bool Type::is_derived() { return mono_class_get_parent(self_class) != nullptr; }
Type Type::GetBaseClass() { return Type(mono_class_get_parent(self_class)); }
bool Type::is_value_type() { return value_type; }
unsigned int Type::get_sizeof() { return size; }
unsigned int Type::get_alignof() { return alignof_; }

std::vector<Property> Type::GetProperties() const {
	void* iter = nullptr;
	MonoProperty* prop = mono_class_get_properties(self_class, &iter);
	std::vector<Property> vec;
	while (prop != nullptr) {
		vec.emplace_back(prop);
		prop = mono_class_get_properties(self_class, &iter);
	}

	return vec;
}

std::vector<Function> Type::GetMethods() const {
	void* iter = nullptr;
	MonoMethod* method = mono_class_get_methods(self_class, &iter);
	std::vector<Function> vec;

	while (method != nullptr) {
		vec.emplace_back(method);
		method = mono_class_get_methods(self_class, &iter);
	}

	return vec;
}

//Field Type::GetField(const std::string& name) const {
//
//}

Function Type::GetFunction(const std::string& name, int arg_count) const {
	return Function(*this, name, arg_count);
}

Function Type::GetFunction(const std::string& name_with_args) const {
	return Function(*this, name_with_args)
}

MonoClass* Type::get_class_ptr() const { return self_class; }
MonoType* Type::get_type_ptr() const { return self_type; }

const std::string& Type::get_namespace() const { return _namespace; }
const std::string& Type::get_fullname() const { return fullname; }
const std::string& Type::get_name() const { return name; }

