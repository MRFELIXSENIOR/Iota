#pragma once

#include "IotaEvent.hpp"
#include "IotaBehavior.hpp"

#include <map>
#include <unordered_map>
#include <string>
#include <type_traits>
#include <optional>
#include <concepts>
#include <typeinfo>
#include <cstdint>

namespace iota {
	class Renderer;
	class Window;

	namespace Mono {
		class Script;
	};

	namespace GameInstance {
		class Instance;

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
			Property<T>& operator=(T rhs) { set(rhs); }
			Event::EventSignal<T>& GetValueChangedSignal() { return signal; }

			bool operator==(const T& rhs) { return (value == rhs); }
			bool operator!=(const T& rhs) { return (value != rhs); }
		};

		template<typename T, typename P>
		concept IsProperty = std::is_base_of_v<Property<P>, T>;

		template <typename T>
		concept IsInstance = std::is_base_of_v<Instance, T>;

		struct type_info_hash {
			std::size_t operator()(const std::type_info* type) const {
				return type->hash_code();
			}
		};

		struct type_info_compare_equal {
			bool operator()(const std::type_info* first, const std::type_info* second) const {
				return *first == *second;
			}
		};

		class Instance : protected GameBehavior {
		public:
			Property<std::string> name;

			Instance();
			~Instance();

			virtual void Load();
			virtual void Render();
			virtual void Update();

			template <IsInstance T>
			void SetParent(T& p) {
				Instance* p_new = static_cast<Instance*>(&p);

				parent_changed.Fire(*p_new);
				parent = p_new;
			}

			template <IsInstance T>
			T& GetParent() {
				return *dynamic_cast<T*>(parent);
			}

			template <IsInstance T>
			void AddChildren(T& c) {
				Instance* c_new = static_cast<Instance*>(&c);

				child_added.Fire(*c_new);

				auto&& pair = std::make_pair(&typeid(T), c_new);
				children.insert(pair);

				c_new->SetParent(*this);
			}

			template <IsInstance T, typename... Args>
			void CreateChildren(Args... args) {
				T* c_new = new T(args...);
				AddChildren(*c_new);
			}

			template <IsInstance T>
			std::optional<T&> GetChildren(const std::string& name) {
				const std::type_info* key = &typeid(T);
				auto rg = children.equal_range(key);

				for (auto it = rg.first; it != rg.second; ++it) {
					if (it->second.name == name) {
						return std::make_optional(*dynamic_cast<T*>(it->second));
					}
				}

				return std::nullopt;
			}

			Event::EventSignal<Instance&> parent_changed;
			Event::EventSignal<Instance&> child_added;

		private:
			Instance* parent;
			std::unordered_multimap<std::type_info*, Instance*, type_info_hash, type_info_compare_equal> children;

			std::vector<Mono::Script*> attached_scripts;
			friend class Mono::Script;
		};
	} // namespace GameInstance
} // namespace iota
