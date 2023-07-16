#pragma once

#include "IotaGameInstance.hpp"

#include <sol/sol.hpp>
#include <string>
#include <exception>

namespace iota {
	namespace Lua {
		struct Script : GameInstance::Instance {
		private:
			GameInstance::Property<std::string> file_name;
			bool attached;
			std::string attached_instance_name;
			friend class GameInstance::Instance;

		public:
			Script(std::string file);
			~Script();

			void Run();
			template <GameInstance::IsInstance T>
			void Attach(T& inst) {
				attached = true;

				attached_instance_name = inst.name.data();
				Lua::GetState()[attached_instance_name] = &inst;
				inst.attached_scripts.push_back(this);
			}

			sol::protected_function_result execution_result;
		};

		void LoadSTD();

		sol::state& GetState();

		sol::table& GetIota();
		sol::table& GetEnum();

		void RunAllScript();
	};
};