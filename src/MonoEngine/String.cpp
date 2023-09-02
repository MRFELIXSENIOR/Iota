#include "String.hpp"

#include <mono/metadata/appdomain.h>
#include <mono/metadata/object.h>

using namespace iota;
using namespace Mono;

String::String(const Object& obj): Object(obj) {}
String::String(const std::string& str) :
	Object(reinterpret_cast<MonoObject*>(mono_string_new(mono_get_root_domain(), str.c_str()))) {}

const std::string& String::utf8() const {
	MonoString* mstr = mono_object_to_string(get_mono_ptr(), nullptr);
	char* raw = mono_string_to_utf8(mstr);
	std::string ret(raw);
	mono_free(raw);
	return ret;
}

const std::u16string& String::utf16() const {
	MonoString* mstr = mono_object_to_string(get_mono_ptr(), nullptr);
	wchar_t* raw = mono_string_to_utf16(mstr);
	std::u16string ret = reinterpret_cast<std::u16string::value_type*>(raw);
	mono_free(raw);
	return ret;
}

const std::u32string& String::utf32() const {
	MonoString* mstr = mono_object_to_string(get_mono_ptr(), nullptr);
	uint32_t* raw = mono_string_to_utf32(mstr);
	std::u32string ret = reinterpret_cast<std::u32string::value_type*>(raw);
	mono_free(raw);
	return ret;
}

