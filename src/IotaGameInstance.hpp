#pragma once

#include "IotaEvent.hpp"
#include "IotaVector.hpp"
#include "IotaApplication.hpp"

#include <memory>
#include <optional>
#include <string>
#include <typeinfo>
#include <map>
#include <iostream>
#include <format>

namespace IotaEngine {
	namespace GameInstance {
		template <typename T>
		concept IsInstance = std::is_base_of_v<Instance, T>;

		class Instance {
		private:
			std::vector<Instance*> children;
			Instance* parent;

		public:
			std::string name;

			Instance();
			~Instance();

			Event::EventSignal<> changed;
			Event::EventSignal<Instance*> child_added;
			Event::EventSignal<Instance*> parent_changed;
			Event::EventSignal<> destroying;

			virtual void Destroy();
			std::vector<Instance*> GetChildren();
			void ClearAllChildren();

			template <IsInstance T>
			std::optional<T*> AddChildren(T* inst) {
				if (static_cast<std::remove_pointer_t<decltype(this)>>(inst) = this) return std::nullopt;
				
				inst.SetParent(this);
				children.push_back(inst);
				child_added.Fire(inst);

				return inst;
			}

			template <IsInstance T>
			std::optional<T*> GetChild(std::string_view child_name) {
				for (Instance* inst : children) {
					if (inst->name == child_name) {
						return dynamic_cast<T*>(inst);
					}
				}

				Application::ThrowException(std::format("Cannot Find {} in {}", child_name, name), Application::Exception::CANNOT_FIND_CHILDREN);
				return std::nullopt;
			}

			template <IsInstance T>
			void SetParent(T* inst) { 
				if (static_cast<std::remove_pointer_t<decltype(this)>>(inst) == this) return;
				parent = &inst;
				parent_changed.Fire(inst);
			}

			virtual void Update();
			virtual void Render();
			virtual void Initialize();
		};

	} // namespace GameInstance
} // namespace IotaEngine
