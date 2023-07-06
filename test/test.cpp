#include "IotaEngine.h"

using namespace IotaEngine;

int main() {
	Application::Initialize("Window Title", 640, 480);
	Lua::Script script("test.lua");
	Application::Start(script);

	return 0;
}