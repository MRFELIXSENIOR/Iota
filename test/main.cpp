#define main SDL_main

#include "script.hpp"
using namespace iota;

#include <exception>

int main(int argc, char** argv) {
	try {
		Application::Initialize("Test Engine", 640, 480);
		REGISTER_MAIN(Main);
		Application::Start();
	}
	catch (std::exception err) {
		std::cout << err.what() << '\n';
	}

	return 0;
}
