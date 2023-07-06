#pragma once

#include "game/IotaGameInstance.hpp"
#include "sol/sol.hpp"

#include <string>

namespace IotaEngine {
	namespace Lua {
		struct Script : GameInstance::Instance {
		private:
			GameInstance::InstanceProperty::Property<std::string> file_name;

		public:
			Script();
			Script(std::string_view file);
			~Script();

			void Run();

			sol::function Load;
			sol::function Update;
			sol::function Render;
		};

		void LoadSTD();
		sol::state& GetState();
	};
};