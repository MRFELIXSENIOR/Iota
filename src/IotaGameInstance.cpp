#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"
#include "IotaScriptEnvironment.hpp"
#include "IotaEvent.hpp"
#include "IotaBasic.hpp"
#include "IotaBehavior.hpp"

#include <map>

using namespace iota;
using namespace GameInstance;

uint64_t GameBehavior::GetID() { return id; }

Instance::Instance(): GameBehavior() {}

Instance::~Instance() {}

void Instance::Load() {}
void Instance::Render() {}
void Instance::Update() {}

void GameInstance::LoadLuaSTD() {
	sol::state& lua = Lua::GetState();
	sol::table& Iota = Lua::GetIota();

	BindPropertyType<std::string>();

	sol::usertype<Instance> instance = lua.new_usertype<Instance>(
		"Instance",
		sol::constructors<Instance()>()
	);

	instance["Name"] = &Instance::name;
}