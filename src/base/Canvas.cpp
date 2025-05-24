#include <base/Canvas.hpp>

SDL_Window* Canvas::CreateSDLWindow() {
	auto* win =
			SDL_CreateWindow("CGScratch", 640, 480, SDL_WINDOW_OPENGL);
	if (!win) {
		SDL_Log("Unable to create window: %s", SDL_GetError());
		SDL_Quit();
	}
	return win;
}

Canvas::Canvas() :
		window(CreateSDLWindow(), SDL_DestroyWindow) {
}

Canvas::~Canvas() {
	window.reset();
    SDL_Quit();//确保window最后析构
}