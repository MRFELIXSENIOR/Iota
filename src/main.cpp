#include "main.h"

#include <exception>

int main() {
	try {
		if (Application::Initialize("Test", 640, 480)) {
			REGISTER_MAIN(Main);
			Application::Start();
		}
	}
	catch (std::exception err) {
		std::cout << err.what() << '\n';
	}

	return 0;
}