#include "IotaEngine.h"

using namespace iota;

int main() {
	Application::Initialize("Test Window", 640, 480);
	Lua::Script script("c:\\Users\\phamt\\Documents\\GitHub\\Iota\\test\\test.lua");
	Application::Start();

	return 0;
}