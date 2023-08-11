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

Assembly::Assembly(const std::string& name) : file_name(name) {}

void SEnv::Initialize(std::vector<std::string>& file_names) {
	domain = mono_jit_init("IotaDomain");

	if (!domain) {
		Application::Throw(ErrorType::RUNTIME_ERROR, "Failed To Initialize Scripting Environment!");
		return;
	}

	std::vector<Assembly> assemblies;
	for (const std::string& n : file_names) {
		assemblies.emplace_back(n);
	}

	Assemble(assemblies);
}

void SEnv::Assemble(std::vector<SEnv::Assembly>& assemblies) {
	std::string cmd = "csc -t:library -out:";

	for (SEnv::Assembly& a : assemblies) {
		std::filesystem::path out_asm(a.file_name);
		out_asm.replace_extension("dll");

		a.generated_assembly = out_asm.string();

		std::string new_cmd = std::format("{}{} {}", cmd, out_asm.string(), a.file_name);
		int result = std::system(new_cmd.c_str());
		if (result != 0) {
			a.assemble_result = false;
			continue;
		}

		a.assembly = mono_domain_assembly_open(domain, out_asm.string().c_str());
		if (!a.assembly) {
			a.assemble_result = false;
			Application::Throw(ErrorType::RUNTIME_ERROR, "Failed To Open Assembly! File: " + a.file_name);
			continue;
		}

		a.img = mono_assembly_get_image(a.assembly);
		if (!a.img) {
			a.assemble_result = false;
			Application::Throw(ErrorType::RUNTIME_ERROR, "Failed To Get Assemblies Image! File: " + a.file_name);
			continue;
		}

		a.assemble_result = true;
	}
}