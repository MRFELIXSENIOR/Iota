#pragma once

#include <filesystem>

#include <mono/metadata/assembly.h>
#include <mono/metadata/image.h>

namespace iota {
	namespace Mono {
		struct TypeClass;

		struct Context final {
		private:
			MonoAssembly* assembly = nullptr;
			MonoImage* img = nullptr;

			std::filesystem::path assembly_path;

		public:
			//explicit Context(const std::string& cs_file);
			explicit Context(std::filesystem::path path_to_assembly);
			explicit Context(MonoAssembly* assembly);
			explicit Context(MonoImage* image);

			TypeClass GetClass(const std::string& name) const;
			TypeClass GetClass(const std::string& name_space, const std::string& name) const;

			MonoAssembly* GetAssembly() const;
			MonoImage* GetImage() const;

			bool IsValid();
		};
	}
}