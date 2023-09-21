#pragma once

#include "IotaGameInstance.hpp"

#include <memory>

namespace iota {
	class Script : public Instance {
	public:
		using Instance::Instance;
		Script();

		virtual void Load() = 0;
		virtual void Render() = 0;
		virtual void Update() = 0;
	};

	template <typename T>
	constexpr bool IsValidScript = std::is_base_of_v<Script, T> && std::is_default_constructible_v<T>;

	//Is used to register the main class to Iota for main script execution
	template <typename T>
	requires IsValidScript<T>
	void RegisterMainClass() {
		static T* c;
		if (c == nullptr) {
			c = new T;
		}
		else return;
	}

	//Is used to register the main class to Iota for main script execution
#define REGISTER_MAIN(CLASS) RegisterMainClass<CLASS>();
}