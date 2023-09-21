#include "main.h"

int main() {
	if (Application::Initialize("Test", 1024, 768)) {
		REGISTER_MAIN(Main);
		Application::Start();
	}

	return 0;
}