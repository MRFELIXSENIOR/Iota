#pragma once

#include <filesystem>

#include <mono/metadata/assembly.h>
#include <mono/metadata/image.h>

namespace iota {
	namespace Mono {
		struct Context final {
		public:
			MonoAssembly* assembly = nullptr;
			MonoImage* img = nullptr;
			std::filesystem::path assembly_path;

			//explicit Context(const std::string& cs_file);
			explicit Context(std::filesystem::path path_to_assembly);
			explicit Context(MonoAssembly* assembly);
			explicit Context(MonoImage* image);
			~Context();

			bool valid();
		};
	}
}