#pragma once

#include "IotaEvent.hpp"
#include "IotaBasic.hpp"
#include "IotaException.hpp"
#include "IotaProperty.hpp"
#include "IotaDef.hpp"

#include <unordered_map>
#include <string>
#include <type_traits>
#include <concepts>
#include <cstdint>

namespace iota {
	namespace Basic {
		void Load();
		void Render();
		void Update(float dt);
		void PollEvent();
	}

	class Script;
	class Window;
	class Instance;

	template <typename T>
	concept IsInstance = std::is_base_of_v<Instance, T>;

	class IOTA_API ActorInterface {
	protected:
		virtual void Load() = 0;
		virtual void Render() = 0;
		virtual void Update(float dt) = 0;

		ActorInterface();

	private:
		using ActorContType = std::vector<ActorInterface*>;

		void Init();
		std::shared_ptr<Window> window;

		static ActorContType& GetCont();

		friend void Basic::PollEvent();
		friend void Basic::Load();
		friend void Basic::Render();
		friend void Basic::Update(float dt);
	};

	class IOTA_API Instance : ActorInterface {
	public:
		using ActorInterface::ActorInterface;
		Instance();
		~Instance();

		template <IsInstance T>
		T& SetParent(T& p) {
			Instance* p_new = static_cast<Instance*>(&p);

			parent_changed.Fire(*p_new);
			parent = p_new;
			return p;
		}

		template <IsInstance T>
		T& GetParent() {
			if (!parent)
				throw RuntimeError("Could not GetParent");
			return *dynamic_cast<T*>(parent);
		}

		template <IsInstance T>
		T& AddChildren(T& child) {
			Instance* c_new = static_cast<Instance*>(&child);
			child_added.Fire(*c_new);

			auto&& pair = std::make_pair(c_new->Name.Value, c_new);
			children.insert(pair);

			c_new->SetParent(*this);
			return child;
		}

		template <IsInstance T, typename... Args>
		T& CreateChildren(Args... args) {
			T* c_new = new T(std::forward<Args>(args)...);
			return AddChildren(*c_new);
		}

		template <IsInstance T>
		T& GetChildren(const std::string& name) {
			if (children.find(name) != children.end())
				return *dynamic_cast<T>(children.at(name));

			throw RuntimeError("Could not GetChildren, name: " + name);
		}

		template <IsInstance T>
		T& operator[](const std::string& name) {
            return GetChildren<T>(name);
		}

		inline bool IsChildren() { return parent != nullptr; }

		template <IsInstance T>
		bool IsChildrenOf(const T& inst) {
			return inst.GetChildren<decltype(*this)>(Name.Value) == *this;
		}

		Property<std::string> Name;

		ScriptSignal<Instance&> ParentChanged;
		ScriptSignal<Instance&> ChildAdded;

	protected:
		virtual void Load();
		virtual void Render();
		virtual void Update(float dt);

	private:
		Instance* parent = nullptr;
		std::unordered_map<std::string, Instance*> children;

		Event<Instance&> parent_changed;
		Event<Instance&> child_added;
	};
} // namespace iota
