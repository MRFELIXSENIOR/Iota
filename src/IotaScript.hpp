#pragma once

#include "IotaGameInstance.hpp"

#include <vector>
#include <filesystem>

namespace iota {
	class Script : protected GameInstance::Instance {
	public:
		Script(const std::filesystem::path& script_path);

		template <GameInstance::IsInstance T>
		void Attach(T& inst) const {
			inst.attached_scripts.push_back(this);
			attached_inst = static_cast<GameInstance::Instance*>(&inst);
		}

		template <GameInstance::IsInstance T>
		T& GetAttachedInstance() const {
			return *dynamic_cast<T*>(attached_inst);
		}

		const std::filesystem::path& GetPath() const;

	private:
		std::filesystem::path script_path;
		GameInstance::Instance* attached_inst;
	};

	const std::vector<std::filesystem::path>& GetScripts();
}