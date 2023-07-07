#pragma once

#include "sol/sol.hpp"
#include "IotaGameInstance.hpp"

#include <string>
#include <exception>

namespace IotaEngine {
	namespace Lua {
		struct Script : GameInstance::Instance {
		private:
			GameInstance::Property<std::string> file_name;

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
		sol::table& GetIota();

		int ErrorHandle(lua_State* L, sol::optional<const std::exception&> maybe_err, sol::string_view desc);
	};
};