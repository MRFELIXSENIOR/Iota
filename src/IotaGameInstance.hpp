#pragma once

#include "IotaEvent.hpp"

#include <optional>
#include <map>
#include <vector>
#include <string>
#include <type_traits>
#include <format>
#include <concepts>
#include <cstdint>
#include <sol/sol.hpp>

namespace iota {
	class Renderer;
	namespace Lua {
		struct Script;
		sol::state& GetState();
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

		class Instance {
		public:
			Property<std::string> name;

			Instance();
			~Instance();

			uint64_t ID();

			virtual void Load();
			virtual void Render();
			virtual void Update();

		private:
			std::vector<Lua::Script*> attached_scripts;
			friend class Lua::Script;

		protected:
			Renderer* renderer;
			uint64_t id;
		};

		template <typename T>
		concept IsInstance = std::is_base_of_v<Instance, T>;

		template <typename T>
		sol::usertype<Property<T>> BindPropertyType() {
			sol::state& lua = Lua::GetState();

			sol::usertype<Property<T>> property = lua.new_usertype<Property<T>>("Property", sol::no_constructor);
			property["Value"] = sol::property(&Property<T>::data, &Property<T>::set);
			property.set("Name", sol::readonly(&Property<T>::property_name));

			Event::BindScriptSignal<T>();

			return property;
		}

		void LoadLuaSTD();

		using InstanceMap = std::map<uint64_t, Instance*>;
		const InstanceMap& GetInstanceMap();
	} // namespace GameInstance
} // namespace iota
