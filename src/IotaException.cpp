#include "IotaException.hpp"
#include "IotaApplication.hpp"
#include "MonoEngine/Object.hpp"
#include "MonoEngine/Property.hpp"

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

Exception::Exception(MonoObject* mono_exc_object) {
	Object obj(mono_exc_object);
	Type type = obj.GetType();

	std::string msg = type.GetProperty("Message").Get();
}

void ExceptionReference::Raise() {
	if (!valid()) return;
	mono_raise_exception(mono_exception);
}

MonoException* ExceptionReference::get_mono_ptr() const {
	return mono_exception;
}

bool ExceptionReference::valid() {
	return mono_exception != nullptr
}
