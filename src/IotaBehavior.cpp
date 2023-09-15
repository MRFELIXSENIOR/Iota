#include "IotaBehavior.hpp"
#include "IotaApplication.hpp"
#include "IotaBasic.hpp"

#include <SDL.h>
#include <cstdint>

using namespace iota;

static ActorMap actor_map;
static uint64_t behav_id;

void ActorMap::Load() const {
	for (auto& a : *this) {
		a.second->Load();
	}
}

void ActorMap::Render() const {
	for (auto& a : *this) {
		a.second->Render();
	}
}

void ActorMap::Update() const {
	for (auto& a : *this) {
		a.second->Update();
	}
}

ActorMap& iota::GetActorMap() { return actor_map; }

void GameBehavior::ActorInit() {
	if (SDL_GetMouseFocus()) {
		actor_window = std::make_shared<Window>(SDL_GetMouseFocus());
		id = behav_id++;
	}
}

GameBehavior::GameBehavior() {
	auto&& a = std::make_pair(id, static_cast<GameBehavior*>(this));
	actor_map.insert(a);
}

GameBehavior::~GameBehavior() {}