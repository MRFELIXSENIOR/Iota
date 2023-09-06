#include "Function.hpp"
#include "TypeClass.hpp"
#include "Context.hpp"
#include "../IotaException.hpp"

#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>

#include <vector>

using namespace iota;
using namespace Mono;

void Function::data() {
	sig = mono_method_signature(self);
	name = mono_method_get_name(self);
	fullname = mono_method_full_name(self, true);
}

Function::Function(MonoMethod* method): self(method) {
	if (!self)
		throw RuntimeError("Cannot Create Mono Function Reference");
	data();
}

Function::Function(const TypeClass& cl_ref, const std::string& fn_name, int argc): self(mono_class_get_method_from_name(cl_ref.GetClassPointer(), fn_name.c_str(), argc)) {
	if (!self)
		throw RuntimeError("Cannot Create Mono Function Reference: " + fn_name + " in class: " + cl_ref.GetName());
	data();
}

Function::Function(const TypeClass& cl_ref, const std::string& fn_name_with_args) {
	MonoMethodDesc* desc = mono_method_desc_new((":" + fn_name_with_args).c_str(), 0);
	self = mono_method_desc_search_in_class(desc, cl_ref.GetClassPointer());
	mono_method_desc_free(desc);
	if (!self)
		throw RuntimeError("Cannot Create Mono Function Reference: " + fn_name_with_args + " in class: " + cl_ref.GetName());
	data();
}

bool Function::IsValid() { return self != nullptr; }
bool Function::IsStatic() {
	unsigned int flags = mono_method_get_flags(self, nullptr);
	return (flags & MONO_METHOD_ATTR_STATIC) != 0;
}

bool Function::IsVirtual() {
	unsigned int flags = mono_method_get_flags(self, nullptr);
	return (flags & MONO_METHOD_ATTR_VIRTUAL) != 0;
}

MonoMethod* Function::GetDataPointer() const { return self; }
MonoMethodSignature* Function::GetSignaturePointer() const { return sig; }
const std::string& Function::GetFullname() const { return fullname; }
const std::string& Function::GetName() const { return name; }

inline AccessModifier filter(unsigned int flags) {
	switch (flags) {
	case MONO_METHOD_ATTR_PUBLIC:
		return AccessModifier::PUBLIC;
	case MONO_METHOD_ATTR_FAMILY:
		return AccessModifier::PROTECTED;
	case MONO_METHOD_ATTR_PRIVATE:
		return AccessModifier::PRIVATE;
	case MONO_METHOD_ATTR_ASSEM:
		return AccessModifier::INTERNAL;
	case MONO_METHOD_ATTR_FAM_AND_ASSEM:
		return AccessModifier::PROTECTED_INTERNAL;
	}
}

AccessModifier Function::GetAccessModifier() const {
	unsigned int flags = mono_method_get_flags(self, nullptr) & MONO_METHOD_ATTR_ACCESS_MASK;
	return filter(flags);
}

const TypeClass& Function::GetReturnType() const {
	MonoType* rt = mono_signature_get_return_type(sig);
	return TypeClass(rt);
}

const std::vector<TypeClass>& Function::GetArgTypes() const {
	std::vector<TypeClass> args_t;
	void* iter = nullptr;
	MonoType* type = mono_signature_get_params(sig, &iter);
	while (type != nullptr) {
		args_t.emplace_back(type);
		type = mono_signature_get_params(sig, &iter);
	}

	return args_t;
}
