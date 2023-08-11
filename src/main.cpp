#include "IotaEngine.h"

using namespace iota;

int main(int argc, char** argv) {
	Application::Initialize("Test Window", 640, 480, argc, argv);
	SEnv::Script script("hw.cs");

	script.InvokeLoad();
	Application::Start();

	return 0;
}