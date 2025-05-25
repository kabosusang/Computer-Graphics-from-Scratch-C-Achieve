#pragma once
#include "SDL3/SDL_video.h"
#include <SDL3/SDL.h>
#include <memory>

class Canvas {
public:
	static Canvas& getInstance() {
		static Canvas instance;
		return instance;
	}
	Canvas(const Canvas&) = delete;
	Canvas& operator=(const Canvas&) = delete;

	SDL_Window* GetWindow() { return window.get(); }

	//Window WH
    int GetWindowW(){return WindowW;}
    int GetWindowH(){return WindowH;}

private:
	Canvas();
	~Canvas();
	static SDL_Window* CreateSDLWindow();

private:
	int WindowW = 600;
	int WindowH = 600;
	std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> window;
};