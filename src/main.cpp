#include "IotaEngine.h"

using namespace iota;

int main() {
	bool debug = false;

	if (debug) {
		try {
			Application::Initialize("Test Window", 640, 480);
			Script script("test\\hw.cs");
			Application::Start();
		}
		catch (std::exception exc) {
			std::cout << exc.what() << '\n';
		}
	}
	else {
		Application::Initialize("Test Window", 640, 480);
		Script script("test\\hw.cs");
		Application::Start();
	}

	return 0;
}