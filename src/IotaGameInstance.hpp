#pragma once

#include "IotaEvent.hpp"

#include <map>
#include <vector>
#include <string>
#include <type_traits>
#include <memory>
#include <concepts>
#include <cstdint>
#include <sol/sol.hpp>

namespace iota {
	class Renderer;
	class Window;
	namespace Lua {
		struct Script;
		sol::state& GetState();
	};

	namespace Basic {
		struct GameBehavior {
		protected:
			std::unique_ptr<Renderer> actor_renderer;
			std::unique_ptr<Window> actor_window;

			uint64_t id;

		public:
			virtual void Load() = 0;
			virtual void Render() = 0;
			virtual void Update() = 0;

			uint64_t GetID();
		};

		using ActorMap = std::map<uint64_t, GameBehavior*>;
		const ActorMap& GetActorMap();
	};

	namespace GameInstance {
		template <typename T>
		struct Property {
		private:
			T value;
			Event::EventSignal<T> signal;

		public:
			Property() {}
			explicit Property(T val) : value(val) {}
			~Property() {}

			std::string property_name;

			T data() { return value; }
			void set(T val) { signal.Fire(value); value = val; }
			Property<T>& operator=(T v) { set(v); }
			Event::EventSignal<T>& GetValueChangedSignal() { return signal; }

			bool operator==(const T& rhs) { return (value == rhs); }
			bool operator!=(const T& rhs) { return (value != rhs); }
		};

		template<typename T, typename P>
		concept IsProperty = std::is_base_of_v<Property<P>, T>;

		class Instance : protected Basic::GameBehavior {
		public:
			Property<std::string> name;

			Instance();
			~Instance();

			virtual void Load();
			virtual void Render();
			virtual void Update();

		private:
			std::vector<Lua::Script*> attached_scripts;
			friend class Lua::Script;
		};

		template <typename T>
		concept IsInstance = std::is_base_of_v<Instance, T>;

		template <typename T>
		sol::usertype<Property<T>> BindPropertyType() {
			sol::state& lua = Lua::GetState();

			sol::usertype<Property<T>> property = lua.new_usertype<Property<T>>("Property", sol::no_constructor);
			property["Value"] = sol::property(&Property<T>::data, &Property<T>::set);
			property.set("Name", sol::readonly(&Property<T>::property_name));

			Event::BindScriptSignalType<T>();

			return property;
		}

		void LoadLuaSTD();
	} // namespace GameInstance
} // namespace iota
