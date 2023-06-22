#include "IotaEngine.h"

#include <iostream>
#include <string>

using namespace IotaEngine;

Window window("Title", 640, 480);
Renderer renderer(window);

void iota_main() {
	std::cout << "HI FROM ENGINE\n";
}

int main() {
	Application::InitializeApplication(renderer, window);
	std::cout << "START IOTA\n";

	Application::StartApplication();
	Application::IotaMain(iota_main, renderer, window);
	std::cout << "END IOTA\n";
	return 0;
}
