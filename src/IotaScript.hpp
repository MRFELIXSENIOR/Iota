#pragma once

#include "IotaGameInstance.hpp"
#include "IotaDef.hpp"

#include <memory>

namespace iota {
	class IOTA_API Script : Instance {
	public:
		Script();

	protected:
		virtual void Load() = 0;
		virtual void Render() = 0;
		virtual void Update(float dt) = 0;
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
