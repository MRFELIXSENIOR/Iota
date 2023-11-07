#pragma once

#include "IotaEngine.h"
using namespace iota;

class Main : public Script {
public:

	Main() {}

	void Load() {
		std::cout << "hi\n";
	}

	void Render() {}

	void Update(float dt) {}
};
