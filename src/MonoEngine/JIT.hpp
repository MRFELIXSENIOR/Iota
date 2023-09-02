#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <map>

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

		struct CompileOption {
			std::vector<std::filesystem::path> files; //paths to c# scripts

			std::vector<std::string> references; //references name
			std::vector<std::filesystem::path> ref_locations; //references locations
		};

		struct RunResult {
			std::filesystem::path script_file_path;
			bool result;
		};

		using Result = const std::vector<RunResult>&;
		Result RunScript(const std::vector<std::filesystem::path>& scripts_path, const ExceptionHandler& exc_callback);
		Result RunScript(const std::vector<std::filesystem::path>& scripts_path);

		void Initialize();
		void Clean();
	}
}