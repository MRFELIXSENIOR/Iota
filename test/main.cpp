#include "IotaEngine.h"

using namespace iota;

int main() {
	Application::Initialize("Test Window", 640, 480);
	Lua::Script script("../../test/test.lua");
	Application::Start();

	return 0;
}