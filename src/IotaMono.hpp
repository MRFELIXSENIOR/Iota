#pragma once

#include "IotaGameInstance.hpp"

#include <concepts>
#include <vector>
#include <filesystem>
#include <optional>
#include <string>

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

namespace iota {
	namespace SEnv {
		struct Assembly {
		private:
			bool assemble_result;

		public:
			MonoAssembly* assembly = nullptr;
			MonoImage* img = nullptr;
			std::string file_name;
			std::string generated_assembly;
			
			Assembly(const std::string&);

			bool Assemble();
			bool IsValid();
			bool GetAssembleResult();
		};

		struct Script : public GameInstance::Instance {
		private:
			Assembly assem;
			std::string attached_iname;
			bool attached;
			std::filesystem::path script_path;
			std::string class_name;
			friend class GameInstance::Instance;

		public:
			Script(const std::string& file_name);
			~Script();

			template <GameInstance::IsInstance T>
			void Attach(T& instance) {
				attached = true;
				attached_iname = instance.name.data();
				instance.attached_scripts.push_back(this);
			}

			void InvokeLoad();
		};

		void Initialize(std::vector<std::string>& file_names);
		void Clean();

		MonoClass* GetClass(const std::string& namespace_name, const std::string& class_name, Assembly& assembly);
		MonoClass* GetClass(const std::string& class_name, Assembly& assembly);

		MonoMethod* GetMethod(const std::string& method_name, MonoClass* klass, int arg_count, Assembly& assembly);
	}
};