#include "IotaMono.hpp"
#include "IotaMonoJIT.hpp"
#include "IotaException.hpp"

#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <format>

using namespace iota;
using namespace Mono;

static Domain* domain;

inline std::string quote_wrap(const std::string& str) {
	return "\"" + str + "\"";
}

#define GET_PATH_CSTR(path) (path.string().c_str())

//Context::Context(const std::string& cs_file) {
//	assembly_path = std::filesystem::path(cs_file).replace_extension("dll");
//
//	std::string new_cmd = "csc -t:library -out:" + assembly_path.string() + " " + cs_file;
//	int result = std::system(new_cmd.c_str());
//	if (result != 0) {
//		throw Error("Failed To Generate Assembly! File: " + assembly_path.string());
//	}
//
//	assembly = mono_domain_assembly_open(Domain::GetCurrentDomain().get_mono_ptr(), GET_PATH_CSTR(assembly_path));
//	if (!assembly) {
//		throw Error("Failed To Open Assembly! File: " + assembly_path.string());
//	}
//
//	img = mono_assembly_get_image(assembly);
//	if (!img) {
//		throw Error("Failed To Get Assembly Image! File: " + assembly_path.string());
//	}
//}

Context::Context(std::filesystem::path path_to_assembly) : assembly_path(path_to_assembly) {
	assembly = mono_domain_assembly_open(mono_get_root_domain(), GET_PATH_CSTR(assembly_path));
	if (!assembly) {
		throw RuntimeError("Failed To Open Assembly! File: " + assembly_path.string());
	}

	img = mono_assembly_get_image(assembly);
	if (!img) {
		throw RuntimeError("Failed To Get Assembly Image! File: " + assembly_path.string());
	}
}

Context::Context(MonoAssembly* assem): assembly(assem) {
	if (!assembly) {
		throw RuntimeError("Cannot Create Mono Context with NULL MonoAssembly*");
	}

	img = mono_assembly_get_image(assembly);
	if (!img) {
		throw RuntimeError("Failed To Get Assembly Image! File: " + std::string(mono_image_get_filename(img)));
	}

	assembly_path = std::filesystem::path(mono_image_get_filename(img));
}

Context::Context(MonoImage* image): img(image) {
	if (!img) {
		throw RuntimeError("Cannot Create Mono Context with NULL Assembly Image!");
	}

	assembly = mono_image_get_assembly(image);
	if (!assembly) {
		throw RuntimeError("Failed To Get Assembly! File: " + quote_wrap(mono_image_get_filename(img)));
	}

	assembly_path = std::filesystem::path(mono_image_get_filename(img));
}

Context::~Context() {
	mono_image_close(img);
	mono_assembly_close(assembly);
}

bool Context::valid() {
	return img && assembly;
}

void Class::metadata_init() {
	mono_type = mono_class_get_type(self);
	name = mono_class_get_name(self);
	_namespace = mono_class_get_namespace(self);
	fullname = _namespace.empty() ? name : _namespace + "." + name;
	value_type = mono_class_is_valuetype(self);
}

Class::Class(MonoClass* mono_class): context(mono_class_get_image(mono_class)) {
	self = mono_class;
	if (!self)
		throw Error("Cannot Create Mono Class Reference with NULL MonoClass*");

	metadata_init();
}

Class::Class(MonoType* mono_type): context(mono_class_get_image(mono_type_get_class(mono_type))) {
	self = mono_type_get_class(mono_type);
	if (!self)
		throw RuntimeError("Cannot Create Mono Class Reference with NULL MonoType*");

	metadata_init();
}

Class::Class(const std::string& name_space, const std::string& class_name, Context& ctx): context(ctx) {
	if (!ctx.valid()) {
		throw RuntimeError("Cannot Create Mono Class Reference with Invalid Mono Context!");
	}

	self = mono_class_from_name(ctx.img, name_space.c_str(), class_name.c_str());
	if (!self) {
		std::string msg = "Cannot Create Mono Class Reference! Class Name: " + quote_wrap(class_name);
		throw RuntimeError(msg);
	}

	metadata_init();
}

Class::Class(const std::string& class_name, Context& ctx) : Class("", class_name, ctx) {}
Class::~Class() {}

Object& Class::CreateInstance() {
	return *(new Object(self));
}

bool Class::valid() const {
	return self != nullptr;
}

Function* Class::GetFunctionRef(const std::string& name, int arg_count) {
	for (Function& f : fn) {
		if (f.arg_count == arg_count && f.name == name) {
			return &f;
		}
	}

	return nullptr;
}

MonoClass* Class::get_mono_ptr() const { return self; }
MonoType* Class::get_mono_type_ptr() const { return mono_type; }

const std::string& Class::get_namespace() const { return _namespace; }
const std::string& Class::get_fullname() const { return fullname; }
const std::string& Class::get_name() const { return name; }

Function::Function(MonoMethod* method): self(method) {
	if (!self)
		throw RuntimeError("Cannot Create Mono Function Reference with NULL MonoMethod*");
	desc = mono_method_desc_from_method(self);
	sig = mono_method_signature(self);
	name = mono_method_get_name(self);
	arg_count = mono_signature_get_param_count(sig);
	class_ref = std::make_optional(Class(mono_method_get_class(self)));
}

Function::Function(Class& cl_ref, const std::string& fn_name, int argc) : class_ref(cl_ref), name(fn_name), arg_count(argc) {
	self = mono_class_get_method_from_name((*class_ref).get_mono_ptr(), name.c_str(), arg_count);
	if (!self)
		throw RuntimeError("Cannot Get Mono Function Reference with name: " + quote_wrap(name));
	desc = mono_method_desc_from_method(self);
	sig = mono_method_signature(self);
}

Function::Function(const std::string& fn_name, int argc, Context& ctx) : name(fn_name), arg_count(argc) {
	desc = mono_method_desc_new(name.c_str(), false);
	self = mono_method_desc_search_in_image(desc, ctx.img);
	if (!self)
		throw RuntimeError("Cannot Search for Mono Function Reference with name: " + quote_wrap(name));
	sig = mono_method_signature(self);
}

Function::~Function() {}

bool Function::valid() const {
	return self || class_ref && self;
}
MonoMethod* Function::get_mono_ptr() const { return self; }
MonoMethodDesc* Function::get_mono_desc_ptr() const { return desc; }
MonoMethodSignature* Function::get_mono_sig_ptr() const { return sig; }
const std::string& Function::get_name() const { return name; }

Object::Object(MonoObject* mono_obj): class_ref(mono_object_get_class(mono_obj)), self(mono_obj) {
	if (!self)
		throw RuntimeError("Cannot Create Object Reference with NULL MonoObject*");
}

Object::Object(MonoClass* mono_class): class_ref(mono_class) {
	self = mono_object_new(mono_get_root_domain(), class_ref.get_mono_ptr());
	mono_runtime_object_init(self);
	if (!self)
		throw RuntimeError("Cannot Create Object of class: " + quote_wrap(class_ref.get_name()));
}

Object::Object(Class& cl_ref): class_ref(cl_ref) {
	self = mono_object_new(mono_get_root_domain(), class_ref.get_mono_ptr());
	mono_runtime_object_init(self);
	if (!self)
		throw RuntimeError("Cannot Create Object of class: " + quote_wrap(class_ref.get_name()));
}

MonoObject* Object::get_mono_ptr() const { return self; }
bool Object::valid() {
	return self != nullptr;
}
