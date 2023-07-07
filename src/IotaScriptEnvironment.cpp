#include "IotaScriptEnvironment.hpp"
#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"

using namespace IotaEngine;
using namespace Lua;

static sol::state lua;
static sol::table iota;

Script::Script() {}
Script::Script(std::string_view file) : file_name(file) {}
Script::~Script() {}

void Script::Run() {
	try {
		lua.script_file(file_name.data());
	}
	catch (const sol::error& e) {}
}

void Lua::LoadSTD() {
#define SOL_ALL_SAFETIES_ON 1
	lua.open_libraries(sol::lib::base, sol::lib::package);
	lua.set_exception_handler(&Lua::ErrorHandle);

	iota = lua.create_table();
	lua.set("Iota", iota);
	
	sol::table app = lua.create_table();
	iota["app"] = app;
}

sol::state& Lua::GetState() { return lua; }
sol::table& Lua::GetIota() { return iota; }

int Lua::ErrorHandle(lua_State* L, sol::optional<const std::exception&> maybe_err, sol::string_view desc) {
	std::cerr << "[IOTA] [Script Error Thrown!]\n";
	if (maybe_err) {
		std::cerr << "[Message]: ";
		const std::exception& e = *maybe_err;
		std::cerr << e.what() << '\n';
	}
	else {
		std::cerr << "[Description]: " << desc.data() << '\n';
	}

	return sol::stack::push(L, desc);
}
