#include "IotaEngine.h"

#include <iostream>
#include <string>

using namespace IotaEngine;

void connect(GameInstance::Instance* c) {
	std::cout << "Child Added!, child: " << c->name << '\n';
}

int main() {
	Application::Initialize("funny", 1024, 768);
	Application::Main([]() {
		Texture texture;
		texture.LoadTexture("C:\\Users\\phamt\\Desktop\\funny.png");
		Application::GetRenderer().RenderTextureToScreen(texture);
		Console::Log("This Is A Log Message!");
		Console::Log("Yay a new feature to IotaEngine!!!");
		});
	Application::Start();
	return 0;
}
