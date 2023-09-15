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

struct Info {
	std::string class_name;
	std::string msg;
	std::string stack_trace;
};

Exception::Exception(MonoObject* mono_exc_object): std::runtime_error("") {
	Object obj(mono_exc_object);
	TypeClass klass = obj.GetClass();
	class_name = klass.GetName();

	klass.GetProperty("Message").Get(message);
}

const char* Exception::what() const noexcept {
	return message.c_str();
}

const std::string& Exception::GetClassName() const noexcept { return class_name; }
const std::string& Exception::GetMessage() const noexcept { return message; }

void Mono::DefaultExceptionHandler(const Exception& exc) {
	std::cerr << "[Iota] [Mono Error] [" << exc.GetClassName() << "] " << exc.GetMessage() << '\n';
}

