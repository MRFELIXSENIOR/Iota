#include "IotaMono.hpp"
#include "IotaException.hpp"

#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <format>

using namespace iota;
using namespace Mono;

static Domain* domain = nullptr;
static std::vector<Context> ctx_vec;

Script::Script(const std::string& file_name) : context(file_name), script_path(file_name), attached(false), Load(new Function("Load", 0, context)) {
	std::filesystem::path file(file_name);
	file.replace_extension("");

	class_name = file.string();
}

Script::~Script() {}

void Script::InvokeLoad() {
	Load->Invoke();
}

Context::Context(const std::string& cs_file) {
	assembly_path = std::filesystem::path(cs_file).replace_extension("dll");

	std::string new_cmd = "csc -t:library -out:" + assembly_path.string() + " " + cs_file;
	int result = std::system(new_cmd.c_str());
	if (result != 0) {
		std::string msg = "Failed To Generate Assembly! File: " + assembly_path.string();
		throw Error(msg);
	}

	assembly = mono_domain_assembly_open(domain->get_ptr(), assembly_path.string().c_str());
	if (!assembly) {
		std::string msg = "Failed To Open Assembly! File: " + assembly_path.string();
		throw Error(msg);
	}

	img = mono_assembly_get_image(assembly);
	if (!img) {
		std::string msg = "Failed To Get Assembly Image! File: " + assembly_path.string();
		throw Error(msg);
	}
}

Context::Context(MonoAssembly* assem): assembly(assem) {
	if (!assembly) {
		throw Error("Cannot Get Image From NULL Assembly!");
	}

	img = mono_assembly_get_image(assembly);
	if (!img) {
		std::string msg = "Failed To Get Assembly Image! File: " + std::string(mono_image_get_filename(img));
		throw Error(msg);
	}

	assembly_path = std::filesystem::path(mono_image_get_filename(img));
}

Context::Context(MonoImage* image): img(image) {
	if (!img) {
		throw Error("Cannot Initialize Mono Context with a NULL Assembly Image!");
	}

	assembly = mono_image_get_assembly(image);
	if (!assembly) {
		throw Error("Failed To Get Assembly! File: " + std::string(mono_image_get_filename(img)));
	}

	std::filesystem::path&& path = std::filesystem::path(mono_image_get_filename(img));

	generated_assembly = path
}

bool Context::IsValid() {
	return (img && assembly) ? true : false;
}

Domain::Domain(const std::string& name) {
	dom = mono_domain_create_appdomain(const_cast<char*>(name.c_str()), nullptr);

	mono_bool result = mono_domain_set(dom, 0);
	if (result) {
		mono_thread_attach(dom);
	}
}

Domain::~Domain() {
	if (dom) {
		MonoDomain* root = mono_get_root_domain();
		mono_bool result = mono_domain_set(root, 0);
		if (result) {
			mono_domain_unload(dom);
		}
	}

	mono_gc_collect(mono_gc_max_generation());
}

MonoDomain* Domain::get_ptr() { return dom; }

std::optional<Context> Domain::GetContext(const std::string& path) const {
	auto i = contexts.find(path);
	if (i != contexts.end()) {
		return std::make_optional(i->second);
	}

	return std::nullopt;
}

bool Class::IsValid() const {
	return (self) ? true : false;
}

Class::~Class() {}

Object& Class::CreateInstance() {
	return *new Object(self);
}

Function* Class::GetFunctionRef(const std::string& name, int arg_count) {
	for (Function& f : fn) {
		if (f.arg_count == arg_count && f.name == name) {
			return &f;
		}
	}

	return nullptr;
}

MonoClass* Class::get_ptr() const { return self; }

std::string Class::GetNamespace() const { return _namespace; }
std::string Class::GetName() const { return name; }

Function::~Function() {}

bool Function::IsValid() const {
	return self || cl_ref && self;
}
MonoMethod* Function::get_ptr() const { return self; }

std::string Function::GetName() const { return name; }

MonoObject* Object::get_ptr() const { return self; }

bool Object::IsValid() {
	return self != nullptr;
}

void Mono::Initialize(std::vector<std::string>& file_names) {
	domain = new Domain("IotaMonoDomain");

	//for (const std::string& n : file_names) {
	//	assemblies.emplace_back(n);
	//}

	//for (Mono::Context& a : assemblies) {
	//	a.Assemble();
	//}
}

void Mono::Clean() {
	mono_jit_cleanup(domain->get_ptr());
}