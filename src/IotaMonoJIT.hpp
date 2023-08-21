#pragma once

#include <string>
#include <filesystem>
#include <vector>

#include <mono/utils/mono-logger.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/mono-debug.h>
#include <mono/metadata/exception.h>

#include "IotaException.hpp"

namespace iota {
	namespace Mono {
		namespace JIT {
			void Initialize(bool debug);
			void Clean();
		}

		struct CompileOptions {
			std::vector<std::filesystem::path> files; //paths to c# scripts

			std::vector<std::string> references; //references name
			std::vector<std::filesystem::path> ref_locations; //references locations
		};

		enum class CompilationResult {
			SUCCESS,
			FAILED
		};

		CompilationResult RunScript(const std::vector<std::filesystem::path>& scripts_path, const ExceptionHandler& exc_callback);
		CompilationResult RunScript(const std::vector<std::filesystem::path>& scripts_path);

		void Initialize();
		void Clean();
	}
}