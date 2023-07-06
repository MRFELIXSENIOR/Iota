#pragma once

#include "IotaGameInstance.hpp"
#include "IotaTexture.hpp"
#include "../common/IotaBasic.hpp"
#include "../common/IotaVector.hpp"

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