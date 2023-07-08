#pragma once

#include "IotaEvent.hpp"
#include "IotaException.hpp"
#include "IotaVector.hpp"

#include <optional>
#include <string>
#include <typeinfo>
#include <iostream>
#include <format>

namespace iota {
	namespace GameInstance {
		template <typename T>
		struct Property {
		private:
			T value;
			Event::EventSignal<T> sig;
			friend class Instance;

		public:
			Property() {}
			Property(std::string_view str) : property_name(str) {}
			Property(T val) : value(val) {}
			~Property() {}

			std::string property_name;

			T data() { return value; }
			Property<T>& operator=(T v) {
				sig.Fire(value);
				value = v;
			}
		};

		template <typename T>
		concept IsInstance = std::is_base_of_v<Instance, T>;

		class Instance {
		private:
			std::vector<Instance*> children;
			Instance* parent;

		public:
			Property<std::string> name;

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

				throw Application::Error(std::format("Cannot Find {} in {}", child_name, name));
				return nullptr;
			}

			template <IsInstance T>
			void SetParent(T* inst) {
				if (static_cast<decltype(this)>(inst) == this) return;
				parent = inst;
				parent_changed.Fire(inst);
			}

			template<typename T>
			std::optional<Event::EventSignal<T>> GetPropertyChangedSignal(Property<T>& p) {
				if (!std::is_member_pointer_v<&p>) {
					throw Application::Error(std::format("Cannot Find Property: {}", p.property_name));
					return std::nullopt;
				}

				return p.sig;
			}
		};
	} // namespace GameInstance
} // namespace iota
