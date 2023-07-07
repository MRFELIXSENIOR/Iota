#pragma once

#include "IotaGameInstance.hpp"
#include "IotaTexture.hpp"
#include "IotaBasic.hpp"
#include "IotaVector.hpp"

namespace IotaEngine {
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