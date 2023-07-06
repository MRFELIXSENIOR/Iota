#pragma once

#include "common/IotaEvent.hpp"
#include "common/IotaVector.hpp"
#include "IotaApplication.hpp"

#include <optional>
#include <string>
#include <typeinfo>
#include <iostream>
#include <format>

namespace IotaEngine {
	namespace GameInstance {
		namespace InstanceProperty {
			template <typename T>
			struct Property {
			private:
				T value;
				friend class Instance;

			public:
				Property() {}
				Property(std::string_view str) : property_name(str) {}
				Property(T val) : value(val) {}
				~Property() {}

				Event::EventSignal<T> sig;
				std::string property_name;

				T data() { return value; }
				void set(T v) {
					sig.Fire(value);
					value = v;
				}
			};
		};

		template <typename T>
		concept IsInstance = std::is_base_of_v<Instance, T>;

		class Instance {
		private:
			std::vector<Instance*> children;
			Instance* parent;

		public:
			InstanceProperty::Property<std::string> name;

			Instance();
			~Instance();

			Event::EventSignal<Instance*> child_added;
			Event::EventSignal<Instance*> parent_changed;
			Event::EventSignal<> destroying;

			virtual void Destroy();
			std::vector<Instance*> GetChildren();
			void ClearAllChildren();

			template <IsInstance T>
			T* AddChildren(T* inst) {
				if (this == static_cast<decltype(this)>(inst)) return nullptr;

				inst->SetParent(this);
				children.push_back(static_cast<Instance*>(inst));
				child_added.Fire(inst);

				return inst;
			}

			template <IsInstance T>
			T* GetChild(std::string_view child_name) {
				for (Instance* inst : children) {
					if (inst->name == child_name) {
						return dynamic_cast<T*>(inst);
					}
				}

				IotaEngine::Application::ThrowException(std::format("Cannot Find {} in {}", child_name, name), Application::Exception::FIND_CHILDREN_FAILURE);
				return nullptr;
			}

			template <IsInstance T>
			void SetParent(T* inst) {
				if (static_cast<decltype(this)>(inst) == this) return;
				parent = inst;
				parent_changed.Fire(inst);
			}

			template<typename PropertyType>
			std::optional<Event::EventSignal<PropertyType>> GetPropertyChangedSignal(InstanceProperty::Property<PropertyType>& p) {
				if (!std::is_member_pointer_v<&p>) {
					Application::ThrowException("Cannot Find Property", Application::Exception::FIND_PROPERTY_FAILURE);
					return std::nullopt;
				}

				return p.sig;
			}
		};
	} // namespace GameInstance
} // namespace IotaEngine
