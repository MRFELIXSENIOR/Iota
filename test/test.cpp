#include "IotaEngine.h"

#include <iostream>
#include <string>

using namespace IotaEngine;
long long i = 0;

int main() {
	Application::Initialize("funny", 1024, 768);
	Application::IotaMain([]() {
		Texture texture;
		texture.LoadTexture("C:\\Users\\phamt\\Desktop\\funny.png");
		Application::GetRenderer().RenderTextureToScreen(texture);
		std::cout << i++ << '\n'; 
		});
	Application::Start();
	return 0;
}
