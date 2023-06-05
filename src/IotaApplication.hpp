#pragma once

#include <string_view>

struct SDL_Renderer;
struct SDL_Window;

namespace IotaApplication {

extern SDL_Renderer* AppRenderer;
extern SDL_Window* AppWindow;

extern bool AppRunning;

bool StartApplication();
bool CleanApplication();

void ThrowRuntimeException(std::string_view ErrorTitle, int ErrorCode, std::string_view ErrorMessage);

}
