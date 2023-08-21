#include "IotaEngine.h"

using namespace iota;

int main(int argc, char** argv) {
	Application::Initialize("Test Window", 640, 480, argc, argv);

	try {
		Mono::Script script("hw.cs");
		script.InvokeLoad();
	}
	catch (Mono::Error err) {
		std::cerr << err.what() << '\n';
	}

	Application::Start();

	return 0;
}