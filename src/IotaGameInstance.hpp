#pragma once

#include "IotaEvent.hpp"
#include "IotaException.hpp"

#include <unordered_map>
#include <string>
#include <type_traits>
#include <concepts>
#include <cstdint>

namespace iota {
	namespace Basic {
		void Load();
		void Render();
		void Update();
		void PollEvent();
	}

	class Script;
	class Window;
	class Instance;

	void InternalRegister(Script* base);

	template <typename T>
	requires std::is_assignable_v<T&, T&&>
	struct Property {
	private:
		T value;
		Event<T> signal;
		std::string property_name;

	public:
		Property() = default;
		explicit Property(T val) : value(val) {}

		T Get() const { return value; }

		void Set(const T& val) const {
			signal.Fire(value);
			value = val;
		}

		Property<T>& operator=(const T& rhs) const { Set(rhs); }
		ScriptSignal<T> GetValueChangedSignal() const { return ScriptSignal<T>(signal); }

		const std::string& GetName() const { return property_name; }

		bool operator==(const T& rhs) { return (value == rhs); }
		bool operator!=(const T& rhs) { return (value != rhs); }
	};

	template<typename T, typename PType>
	concept IsProperty = std::is_base_of_v<Property<PType>, T>;

	template <typename T>
	concept IsInstance = std::is_base_of_v<Instance, T>;

	class ActorInterface {
	public:
		virtual void Load() = 0;
		virtual void Render() = 0;
		virtual void Update() = 0;

	protected:
		ActorInterface();

	private:
		using ActorContType = std::vector<ActorInterface*>;

		void Init();
		std::shared_ptr<Window> window;

		static ActorContType& GetCont();

		friend void Basic::PollEvent();
		friend void Basic::Load();
		friend void Basic::Render();
		friend void Basic::Update();
	};

	class Instance : public ActorInterface {
	public:
		using ActorInterface::ActorInterface;
		Instance();
		~Instance();

		template <IsInstance T>
		void SetParent(T& p) {
			Instance* p_new = static_cast<Instance*>(&p);

			parent_changed.Fire(*p_new);
			parent = p_new;
		}

		template <IsInstance T>
		T& GetParent() {
			if (!parent)
				throw RuntimeError("Could not GetParent");
			return *dynamic_cast<T*>(parent);
		}

		template <IsInstance T>
		void AddChildren(T& child) {
			Instance* c_new = static_cast<Instance*>(&child);
			child_added.Fire(*c_new);

			auto&& pair = std::make_pair(c_new->Name.Get(), c_new);
			children.insert(pair);

			c_new->SetParent(*this);
		}

		template <IsInstance T, typename... Args>
		void CreateChildren(Args... args) {
			T* c_new = new T(std::forward<Args>(args)...);
			AddChildren(*c_new);
		}

		template <IsInstance T>
		T& GetChildren(const std::string& name) {
			if (children.find(name) != children.end())
				return *dynamic_cast<T>(children.at(name));

			throw RuntimeError("Could not GetChildren, name: " + name);
		}

		template <IsInstance T>
		T& operator[](const std::string& name) {
			return GetChildren(name);
		}

		Property<std::string> Name;

		ScriptSignal<Instance&> ParentChanged;
		ScriptSignal<Instance&> ChildAdded;

		virtual void Load();
		virtual void Render();
		virtual void Update();

	private:
		Instance* parent;
		std::unordered_map<std::string, Instance*> children;

		Event<Instance&> parent_changed;
		Event<Instance&> child_added;
	};
} // namespace iota
