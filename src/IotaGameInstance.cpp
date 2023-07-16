#include "IotaGameInstance.hpp"
#include "IotaApplication.hpp"
#include "IotaScriptEnvironment.hpp"
#include "IotaEvent.hpp"
#include "IotaBasic.hpp"

#include <map>

using namespace iota;
using namespace GameInstance;

static Basic::ActorMap actor_map;
static uint64_t inst_id;

const Basic::ActorMap& Basic::GetActorMap() { return actor_map; }

uint64_t Basic::GameBehavior::GetID() { return id; }

Instance::Instance() {
	if (SDL_GetKeyboardFocus()) {
		actor_window = std::make_unique<Window>(SDL_GetKeyboardFocus());
		actor_renderer = std::make_unique<Renderer>(*actor_window, true);
		id = inst_id++;
	}

	auto&& a = std::make_pair(id, static_cast<Basic::GameBehavior*>(this));
	actor_map.insert(a);
}

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