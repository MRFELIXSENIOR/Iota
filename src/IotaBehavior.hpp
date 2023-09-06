#pragma once

#include <map>
#include <memory>

namespace iota {
	namespace Event {
		void PollEvent();
	}

	class Window;
	class Renderer;

	struct GameBehavior {
	private:
		void ActorInit();

		friend void Event::PollEvent();
	protected:
		std::shared_ptr<Window> actor_window;
		std::shared_ptr<Renderer> actor_renderer;

		uint64_t id;

		GameBehavior();
		~GameBehavior();

	public:
		virtual void Load() = 0;
		virtual void Render() = 0;
		virtual void Update() = 0;

		uint64_t GetID();
	};

	using ActorMap = std::map<uint64_t, GameBehavior*>;
	ActorMap& GetActorMap();
}