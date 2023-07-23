#pragma once

#include <map>
#include <memory>

#include "IotaBasic.hpp"

namespace iota {
	struct GameBehavior {
	protected:
		std::unique_ptr<Window> actor_window;
		std::unique_ptr<Renderer> actor_renderer;

		uint64_t id;

		GameBehavior();

	public:
		virtual void Load() = 0;
		virtual void Render() = 0;
		virtual void Update() = 0;

		uint64_t GetID();
	};

	using ActorMap = std::map<uint64_t, GameBehavior*>;
	ActorMap& GetActorMap();
}