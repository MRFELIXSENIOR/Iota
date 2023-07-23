#include "IotaBehavior.hpp"
#include "IotaApplication.hpp"
#include "IotaScriptEnvironment.hpp"
#include "IotaBasic.hpp"

#include <SDL.h>
#include <cstdint>

#include <sol/sol.hpp>

using namespace iota;

static ActorMap actor_map;
static uint64_t behav_id;

ActorMap& iota::GetActorMap() { return actor_map; }

GameBehavior::GameBehavior() {
	if (SDL_GetKeyboardFocus()) {
		actor_window = std::make_unique<Window>(SDL_GetKeyboardFocus());
		actor_renderer = std::make_unique<Renderer>(*actor_window, true);
		id = behav_id++;
	}

	auto&& a = std::make_pair(id, static_cast<GameBehavior*>(this));
	GetActorMap().insert(a);
}