#include "IotaScriptEnvironment.hpp"
#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"
#include "IotaEvent.hpp"
#include "IotaKeyboard.hpp"
#include "IotaBasic.hpp"
#include "IotaTexture.hpp"

#include <SDL.h>

#include <map>
#include <thread>
#include <mutex>

using namespace iota;
using namespace Lua;

static sol::state lua;
static sol::table Iota;

static std::mutex script_mutex;
static std::map<std::string, Script*> script_container;

Script::Script(std::string_view file) : file_name(std::string(file.data())) {
	std::pair<std::string, Script*> script = std::make_pair(file.data(), this);
	script_container.insert(script);
}

Script::~Script() {
	script_container.erase(file_name.data());
}

void Script::Run() {
	try {
		std::cout << "file name: " << file_name.data() << '\n';
		lua.script_file(file_name.data());
	}
	catch (const sol::error& e) {}
}

void Lua::LoadSTD() {
	if (Application::IsInitialized()) return;
#define SOL_ALL_SAFETIES_ON 1
	lua.open_libraries(sol::lib::base, sol::lib::package);
	lua.set_exception_handler(&Lua::ErrorHandle);

	std::cout << "[INFO] Lua STD Loaded!\n";

	Iota = lua.create_table();
	lua["Iota"] = Iota;

	Iota["GetWindow"] = &Application::GetWindow;
	Iota["GetRenderer"] = &Application::GetRenderer;

	Iota["Input"] = lua.create_table();
	Iota["Input"]["OnKeyDown"] = lua.create_table();
	Iota["Input"]["OnKeyRelease"] = lua.create_table();

	Iota["Util"] = lua.create_table();

	Iota["Enum"] = lua.create_table();
	Iota["Enum"]["KeyCode"] = lua.create_table();

	Keyboard::LoadLuaSTD();
	Basic::LoadLuaSTD();
	Texture::LoadLuaSTD();
}

sol::state& Lua::GetEngineLuaState() { return lua; }
sol::table& Lua::GetIota() { return Iota; }

int Lua::ErrorHandle(lua_State* L, sol::optional<const std::exception&> maybe_err, sol::string_view desc) {
	std::cerr << "[IOTA] [Script Error!]\n";
	if (maybe_err) {
		std::cerr << "[Message]: ";
		std::cerr << maybe_err.value().what() << '\n';
	}
	else {
		std::cerr << "[Description]: " << desc.data() << '\n';
	}

	return sol::stack::push(L, desc);
}

void run_all_script() {
	std::lock_guard<std::mutex> lock(script_mutex);

	for (auto& s : script_container) {
		s.second->Run();
	}
}

void Lua::RunAllScript() {
	std::thread thread(run_all_script);

	if (thread.joinable()) {
		thread.join();
	}
}
