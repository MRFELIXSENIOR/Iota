#include "IotaApplication.hpp"

#include <cstdlib>
#include <iostream>

#include "SDL2/SDL.h"

#define ERROR_CODE_FAILED_INIT 5

SDL_Renderer* IotaApplication::AppRenderer = nullptr;
SDL_Window* IotaApplication::AppWindow = nullptr;
bool IotaApplication::AppRunning = false;

bool IotaApplication::StartApplication() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        ThrowRuntimeException("Failed To Initialize", ERROR_CODE_FAILED_INIT, SDL_GetError());
        return false;
    }

    AppRunning = true;
    return true;
}

void IotaApplication::ThrowRuntimeException(std::string_view ErrorTitle, int ErrorCode, std::string_view ErrorMessage) {
    std::cout << "Exception Thrown! Code: " << ErrorCode << "\nTitle: " << ErrorTitle << "\nMessage: " << ErrorMessage << '\n';
    std::exit(ErrorCode);
    __builtin_unreachable();
}
