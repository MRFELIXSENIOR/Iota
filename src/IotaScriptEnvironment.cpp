#include "IotaScriptEnvironment.hpp"
#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"

#include <map>
#include <thread>
#include <mutex>

using namespace iota;
using namespace Lua;

static sol::state lua;
static sol::table iota;

static std::mutex script_mutex;
static std::map<std::string, Script*> script_container;

Script::Script(std::string_view file) : file_name(file) {
	std::pair<std::string, Script*> script = std::make_pair(file.data(), this);
	script_container.insert(script);
}

Script::~Script() {
	script_container.erase(file_name.data());
}

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
}

sol::state& Lua::GetState() { return lua; }
sol::table& Lua::GetIota() { return iota; }

int Lua::ErrorHandle(lua_State* L, sol::optional<const std::exception&> maybe_err, sol::string_view desc) {
	std::cerr << "[IOTA] [Script Error!]\n";
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

void run_all() {
	std::lock_guard<std::mutex> lock(script_mutex);

	for (auto& s : script_container) {
		s.second->Run();
	}
}

void Lua::RunAllScript() {
	std::thread thread(run_all);
}
