#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"
#include "IotaScriptEnvironment.hpp"
#include "IotaEvent.hpp"
#include "IotaBasic.hpp"

#include <map>

using namespace iota;
using namespace GameInstance;

static InstanceMap inst_map;
const InstanceMap& GameInstance::GetInstanceMap() { return inst_map; }

static uint64_t inst_id;

Instance::Instance(): renderer(&Application::GetRenderer()) {
	id = inst_id++;

	if (std::is_same_v<decltype(this), Instance*>) {
		inst_map.insert(std::make_pair(id, this));
	}
	else {
		inst_map.insert(std::make_pair(id, static_cast<Instance*>(this)));
	}
}

Instance::~Instance() {}

uint64_t Instance::ID() { return id; }

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