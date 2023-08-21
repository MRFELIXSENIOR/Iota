#include "IotaException.hpp"
#include "IotaApplication.hpp"
#include "IotaMono.hpp"
#include "IotaMonoJIT.hpp"

#include <iostream>
#include <unordered_map>

#include <mono/jit/jit.h>
#include <mono/metadata/exception.h>
#include <mono/metadata/assembly.h>

using namespace iota;
using namespace Mono;

MonoDomain* exc_domain = mono_get_root_domain();
MonoAssembly* exc_assembly = mono_domain_assembly_open(exc_domain, "iota.Exceptions.dll");
MonoImage* exc_img = mono_assembly_get_image(exc_assembly);

Exception::Exception(const std::string& n, const std::string& msg) : name(n), message(msg) {
	mono_exception = mono_exception_from_name(exc_img, "iota.Exceptions", name.c_str());
	if (!mono_exception)
		throw RuntimeError("Cannot Get Exception Reference from Name: " + name);
}

Exception::Exception(MonoObject* exception) {
	if (mono_class_get_image(mono_object_get_class(exception)) != exc_img)
		throw RuntimeError("Cannot Get Exception Reference from MonoObject* loaded in a different image");

	MonoObject* exc_obj = mono_object_isinst(exception, mono_get_exception_class());
	MonoClass* exc_class = mono_object_get_class(exc_obj);
	name = mono_class_get_name(exc_class);
	
	mono_exception = mono_exception_from_name(exc_img, "iota.Exceptions", name.c_str());
	if (!mono_exception)
		throw RuntimeError("Cannot Get Exception Reference from MonoObject* of class: " + std::string(mono_class_get_name(exc_class)));

	MonoProperty* msg_prt = mono_class_get_property_from_name(exc_class, "Message");
	MonoMethod* get = mono_property_get_get_method(msg_prt);
	MonoString* str = mono_object_to_string(mono_runtime_invoke(get, exception, nullptr, nullptr), nullptr);
	message = mono_string_to_utf8(str);
}

void Exception::Raise(const std::string& name, const std::string& msg) {
	Exception exc(name, msg);
	if (!exc.mono_exception) return;

	mono_raise_exception(exc.mono_exception);
}

void Exception::Raise(MonoObject* exception) {
	Exception exc(exception);
	if (!exc.mono_exception) return;

	mono_raise_exception(exc.mono_exception);
}

const char* Exception::what() const noexcept {
	return message.c_str();
}

const char* Exception::get_name() const noexcept {
	return name.c_str();
}

MonoException* Exception::get_mono_ptr() const {
	return mono_exception;
}
