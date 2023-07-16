#include "IotaScriptEnvironment.hpp"
#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"
#include "IotaEvent.hpp"
#include "IotaKeyboard.hpp"
#include "IotaBasic.hpp"
#include "IotaTexture.hpp"
#include "IotaModel.hpp"

#include <SDL.h>

#include <map>
#include <thread>
#include <mutex>

using namespace iota;
using namespace Lua;

static sol::state lua;

static sol::table Iota;
static sol::table Enum;

static std::mutex script_mutex;
static std::map<std::string, Script*> script_container;

void ErrorHandle(sol::optional<std::string> maybe_msg) {
	if (maybe_msg) {
		std::cerr << "[IOTA] [Script Error!]\n" << "[Message]: " << maybe_msg.value() << '\n';
	}
}

int EngineExceptionHandle(lua_State* L, sol::optional<const std::exception&> maybe_err, sol::string_view desc) {
	std::cerr << "[IOTA] [Script Engine Error!]\n";
	if (maybe_err) {
		std::cerr << "[Message]: ";
		std::cerr << maybe_err.value().what() << '\n';
	}
	else {
		std::cerr << "[Description]: " << desc.data() << '\n';
	}

	return sol::stack::push(L, desc);
}

sol::protected_function_result ScriptErrorHandle(lua_State* L, sol::protected_function_result prtf) {
	if (!prtf.valid()) {
		sol::error err = prtf;
		ErrorHandle(sol::make_optional(err.what()));
	}

	return prtf;
}

Script::Script(std::string file) : file_name(file) {
	auto&& script = std::make_pair(file_name.data(), this);
	script_container.insert(script);
}

Script::~Script() {
	script_container.erase(file_name.data());
}

void Script::Run() {
	execution_result = lua.script_file(file_name.data(), ScriptErrorHandle);
	if (attached) {
		lua[attached_instance_name] = sol::lua_nil;
	}
}

void Lua::LoadSTD() {
	if (Application::IsInitialized()) return;
#define SOL_ALL_SAFETIES_ON 1
	lua.open_libraries(sol::lib::base, sol::lib::package);

	lua.set_exception_handler(&EngineExceptionHandle);
	lua.set_panic(sol::c_call<decltype(&ErrorHandle), &ErrorHandle>);

	std::cout << "[INFO] Lua STD Loaded!\n";

	Iota = lua.create_table();
	Enum = lua.create_table();

	lua["Iota"] = Iota;
	lua["Enum"] = Enum;

	Iota["GetWindow"] = &Application::GetWindow;
	Iota["GetRenderer"] = &Application::GetRenderer;

	Iota["Util"] = lua.create_table();

	Event::LoadLuaSTD();
	Basic::LoadLuaSTD();
	Keyboard::LoadLuaSTD();
	GameInstance::LoadLuaSTD();
	Model::LoadLuaSTD();
	Texture::LoadLuaSTD();
}

sol::state& Lua::GetState() { return lua; }
sol::table& Lua::GetIota() { return Iota; }
sol::table& Lua::GetEnum() { return Enum; }

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
