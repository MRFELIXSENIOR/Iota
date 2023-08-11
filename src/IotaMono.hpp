#pragma once

#include <vector>
#include <string>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace iota {
	namespace SEnv {
		struct Assembly {
			MonoAssembly* assembly = nullptr;
			MonoImage* img = nullptr;
			std::string file_name;
			std::string generated_assembly;
			bool assemble_result;
			
			Assembly(const std::string&);
		};

		void Initialize(std::vector<std::string>& file_names);
		void Assemble(std::vector<Assembly>& assemblies);
	}
};