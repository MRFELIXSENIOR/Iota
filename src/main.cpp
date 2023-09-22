#include "main.h"

#include <exception>

int main() {

	try {
		if (Application::Initialize("Test", 1024, 768)) {
			RegisterMainClass<Main>();
			Application::Start();
		}
	}
	catch (std::exception err) {
		std::cout << err.what() << '\n';
	}

	return 0;
}