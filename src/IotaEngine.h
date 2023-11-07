#pragma once

#include "IotaApplication.hpp"
#include "IotaGameInstance.hpp"
#include "IotaInput.hpp"
#include "IotaEvent.hpp"
#include "IotaBasic.hpp"
#include "IotaSound.hpp"
#include "IotaTexture.hpp"
#include "IotaVector.hpp"
#include "IotaException.hpp"
#include "IotaFont.hpp"
#include "IotaCamera.hpp"
#include "IotaScript.hpp"

#include "IotaSprite.hpp"
#include "IotaTile.hpp"
#include "IotaUIObject.hpp"

namespace iota {
	template <typename T>
	void Print(T out) { std::cout << out << '\n'; }

	template <typename... Args>
	void Print(Args... args) { (print(std::forward<Args>(args)), ...); }
}
