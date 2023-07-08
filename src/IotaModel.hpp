#pragma once

#include "IotaGameInstance.hpp"
#include "IotaTexture.hpp"
#include "IotaBasic.hpp"
#include "IotaVector.hpp"

namespace iota {
	namespace Model {
		class Part : GameInstance::Instance {
		private:
			
		public:
			Part();
			Part(Renderer& renderer);
			Part(Texture& texture);
		};
	};
};