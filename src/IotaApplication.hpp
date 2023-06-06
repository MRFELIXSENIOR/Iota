#pragma once

#include <string_view>
#include "IotaException.hpp"

struct SDL_Renderer;
struct SDL_Window;

namespace IotaEngine {
namespace IotaApplication {

extern SDL_Renderer* AppRenderer;
extern SDL_Window* AppWindow;

extern bool AppRunning;

bool StartApplication(std::string_view WindowTitle, int WindowWidth, int WindowHeight);
bool CleanApplication();

void ThrowRuntimeException(std::string_view ErrorTitle, IotaException ErrorCode, std::string_view ErrorMessage);
void ThrowRuntimeException(std::string_view ErrorTitle, IotaException ErrorCode);
void ThrowRuntimeException(IotaException ErrorCode);

};
};
