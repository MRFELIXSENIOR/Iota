#pragma once

#include "IotaEvent.hpp"
#include "IotaVector.hpp"

#include <array>
#include <bitset>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

namespace IotaEngine {
	namespace GameComponent {

		class Component;

#define MAX_COMPONENT_SIZE 32
#define SAME_COMPONENT_TYPE -1

		typedef int ComponentID;
		using ComponentBitset = std::bitset<MAX_COMPONENT_SIZE>;
		using ComponentContainer = std::array<Component*, MAX_COMPONENT_SIZE>;
		using ComponentDynamicContainer = std::vector<std::unique_ptr<Component>>;

		template <typename T>
		concept IsComponent = std::is_base_of_v<Component, T>;

		class Component {
		private:
			friend class GameInstance::Instance;
			GameInstance::Instance* component_owner;
			ComponentID id;
			bool owned;
			std::string name;

		public:
		};

		template <IsComponent T> struct GetComponentTypeID {
			static std::map<ComponentID, const std::type_info*> given_type;

			static const ComponentID value = []() {
				for (auto& t : given_type) {
					if (typeid(T) == *t.second)
						return t.first;
				}

				static ComponentID id = 0;
				given_type.insert(std::make_pair(id++, &typeid(T)));
				return id;
			}();
		};

		template <typename T>
		static inline constexpr ComponentID get_component_typeid_v =
			GetComponentTypeID<std::remove_cv_t<std::remove_reference_t<T>>>::value;

	} // namespace GameComponent

	namespace GameInstance {

		class Instance {
		private:
			std::vector<Instance*> children;
			Instance* parent;

			GameComponent::ComponentBitset component_bitset;
			GameComponent::ComponentContainer component_container;
			GameComponent::ComponentDynamicContainer dyn_component_container;

		public:
			Event::EventSignal<>* changed;
			Event::EventSignal<Instance*>* child_added;
			Event::EventSignal<Instance*>* parent_changed;
			Event::EventSignal<>* destroying;

			std::string class_name;
			std::string name;

			Instance();
			Instance(std::string_view name);
			~Instance();

			virtual void Destroy();
			virtual std::vector<Instance*>& GetChildren();
			virtual void ClearAllChildren();
			Instance* FindFirstChild(std::string_view name);

			void AddChildren(Instance* instance);
			void SetParent(Instance* instance);

			template <GameComponent::IsComponent T, typename... Args>
			std::optional<T> AddComponent(Args... args) {
				if (GameComponent::get_component_typeid_v<T> <=
					component_container[component_container.size() - 1]->id)
					return std::nullopt;
				T* component = new T(args...);
				component->component_owner = this;
				component->owned = true;
				component->id = GameComponent::get_component_typeid_v<T>;
				std::unique_ptr<GameComponent::Component> cmp_ptr = component;

				dyn_component_container.push_back(std::move(cmp_ptr));
				component_bitset[GameComponent::get_component_typeid_v<T>] = true;
				component_container[GameComponent::get_component_typeid_v<T>] =
					static_cast<GameComponent::Component*>(component);

				return *component;
			}

			template <GameComponent::IsComponent T> std::optional<T> GetComponent() {
				if (component_bitset[GameComponent::get_component_typeid_v<T>] ==
					true) {
					return *dynamic_cast<T>(
						component_container[GameComponent::get_component_typeid_v<T>]);
				}
				else
					return std::nullopt;
			}
		};

	} // namespace GameInstance
} // namespace IotaEngine
