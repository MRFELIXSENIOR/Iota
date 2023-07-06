#include "IotaScriptEnvironment.hpp"
#include "game/IotaApplication.hpp"

using namespace IotaEngine;
using namespace Lua;

static sol::state lua;

Script::Script() {}
Script::Script(std::string_view file) : file_name(file) {}
Script::~Script() {}

void Script::Run() {
	try {
		lua.script_file(file_name.data());
	}
	catch (const sol::error& e) {
		Application::ThrowException("Script Error", Application::Exception::LUA_SCRIPT_ERROR, e.what());
	}
}

void Lua::LoadSTD() {
	lua.open_libraries(sol::lib::base, sol::lib::package);

	sol::table iota = lua.create_table();
	lua.set("Iota", iota);
	
	sol::table app = lua.create_table();
	iota["app"] = app;

	app.set_function("test", &Application::test);
}

sol::state& Lua::GetState() { return lua; }
