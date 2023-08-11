#include "IotaMono.hpp"
#include "IotaException.hpp"

#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <format>

using namespace iota;
using namespace SEnv;

static MonoDomain* domain = nullptr;
static std::string cmd = "csc -t:library -out:";
static std::vector<Assembly> assemblies;

Script::Script(const std::string& file_name) : assem(file_name), script_path(file_name), attached(false) {
	std::filesystem::path file(file_name);
	file.replace_extension("");

	class_name = file.string();
}
Script::~Script() {}

void Script::InvokeLoad() {
	MonoClass* klass = GetClass(class_name, assem);
	MonoMethod* load = GetMethod("Load", klass, 0, assem);

	if (!load) {
		Application::Throw(ErrorType::RUNTIME_ERROR, "Failed To Invoke Load Function!");
		return;
	}

	mono_runtime_invoke(load, klass, nullptr, nullptr);
}

Assembly::Assembly(const std::string& name) : file_name(name), assemble_result(false) { Assemble(); }

bool Assembly::Assemble() {
	if (GetAssembleResult() == true)
		return false;

	std::filesystem::path out_asm(file_name);
	out_asm.replace_extension("dll");

	generated_assembly = out_asm.string();
	std::string new_cmd = cmd + out_asm.string() + " " + file_name;

	int result = std::system(new_cmd.c_str());
	if (result != 0) {
		Application::Throw(ErrorType::ERROR, "Failed To Generate Assembly! File: " + file_name);
		assemble_result = false;
		return false;
	}

	assembly = mono_domain_assembly_open(domain, generated_assembly.c_str());
	if (!assembly) {
		Application::Throw(ErrorType::ERROR, "Failed To Open Assembly! File: " + file_name);
		assemble_result = false;
		return false;
	}

	img = mono_assembly_get_image(assembly);
	if (!img) {
		Application::Throw(ErrorType::ERROR, "Failed To Get Assembly Image! File: " + file_name);
		assemble_result = false;
		return false;
	}

	assemble_result = true;
	return true;
}

bool Assembly::IsValid() {
	return (img && assembly) ? true : false;
}

bool Assembly::GetAssembleResult() { return assemble_result; }

void SEnv::Initialize(std::vector<std::string>& file_names) {
	domain = mono_jit_init("IotaDomain");

	if (!domain) {
		Application::Throw(ErrorType::RUNTIME_ERROR, "Failed To Initialize Scripting Environment!");
		return;
	}

	//for (const std::string& n : file_names) {
	//	assemblies.emplace_back(n);
	//}

	//for (SEnv::Assembly& a : assemblies) {
	//	a.Assemble();
	//}
}

void SEnv::Clean() {
	mono_jit_cleanup(domain);
}

MonoClass* SEnv::GetClass(const std::string& namespace_name, const std::string& class_name, Assembly& assembly) {
	if (!assembly.IsValid()) return nullptr;
	MonoClass* klass = mono_class_from_name(assembly.img, namespace_name.c_str(), class_name.c_str());
	if (!klass) {
		Application::Throw(ErrorType::ERROR, "Failed To Get Class From Name: " + class_name);
		return nullptr;
	}
}

MonoClass* SEnv::GetClass(const std::string& class_name, Assembly& assembly) {
	return GetClass("", class_name, assembly);
}

MonoMethod* SEnv::GetMethod(const std::string& name, MonoClass* klass, int arg_count, Assembly& assembly) {
	if (!assembly.IsValid()) return nullptr;
	MonoMethod* method = mono_class_get_method_from_name(klass, name.c_str(), arg_count);
	if (!method) {
		Application::Throw(ErrorType::ERROR, "Failed To Get Method From Name: " + name);
		return nullptr;
	}

	return method;
}