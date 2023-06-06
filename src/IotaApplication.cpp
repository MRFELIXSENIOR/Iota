#include "IotaApplication.hpp"

#include <cstdlib>
#include <iostream>

#include "SDL2/SDL.h"

using namespace IotaEngine;
using namespace IotaApplication;

SDL_Renderer* IotaApplication::AppRenderer = nullptr;
SDL_Window* IotaApplication::AppWindow = nullptr;
bool IotaApplication::AppRunning = false;

bool IotaApplication::StartApplication(std::string_view WindowTitle, int WindowWidth, int WindowHeight) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        ThrowRuntimeException("Failed To Initialize", INITIALIZATION_FAILURE, SDL_GetError());
        return false;
    }

    AppWindow = SDL_CreateWindow(WindowTitle.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindowWidth, WindowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN );
    if (AppWindow == nullptr) {
        SDL_Quit();
        ThrowRuntimeException("Failed To Create Window!", WINDOW_CREATION_FAILURE, SDL_GetError());
        return false;
    }

    AppRenderer = SDL_CreateRenderer(AppWindow, -1, SDL_RENDERER_ACCELERATED);
    if (AppRenderer == nullptr) {
        SDL_Quit();
        ThrowRuntimeException("Failed To Create Renderer!", RENDERER_CREATION_FAILURE, SDL_GetError());
        return false;
    }

    AppRunning = true;
    return true;
}

bool IotaApplication::CleanApplication() {
    if (AppRunning == false)
        return false;

    SDL_DestroyRenderer(AppRenderer);
    SDL_DestroyWindow(AppWindow);
    SDL_Quit();
    return true;
}

void IotaApplication::ThrowRuntimeException(std::string_view ErrorTitle, IotaException ErrorCode, std::string_view ErrorMessage) {
    std::cout << "Runtime Exception Thrown!\nException Code: " << ErrorCode << "\nTitle: " << ErrorTitle << "\nMessage: " << ErrorMessage << '\n';
    CleanApplication();
    std::exit(ErrorCode);
    __builtin_unreachable();
}

void IotaApplication::ThrowRuntimeException(std::string_view ErrorTitle, IotaException ErrorCode) {
    std::cout << "Runtime Exception Thrown!\nException Code: " << ErrorCode << "\nTitle: " << ErrorTitle << '\n';
    CleanApplication();
    std::exit(ErrorCode);
    __builtin_unreachable();
}

void IotaApplication::ThrowRuntimeException(IotaException ErrorCode) {
    std::cout << "Runtime Exception Thrown!\nException Code: " << ErrorCode << '\n';
    CleanApplication();
    std::exit(ErrorCode);
    __builtin_unreachable();
}
