#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_surface.h"
#include <base/Canvas.hpp>

SDL_Window* Canvas::CreateSDLWindow() {
	auto* win =
			SDL_CreateWindow("CGScratch", 600, 600, SDL_WINDOW_OPENGL);
	if (!win) {
		SDL_Log("Unable to create window: %s", SDL_GetError());
		SDL_Quit();
	}
	return win;
}

Canvas::Canvas() :
		window(CreateSDLWindow(), SDL_DestroyWindow) {
    SDL_Surface* ico = SDL_LoadBMP("../../screenshot/programe.bmp");
    if (ico){
        SDL_SetWindowIcon(window.get(), ico);
        SDL_DestroySurface(ico);
    }
    
}

Canvas::~Canvas() {
	window.reset();
  
    SDL_Quit();//确保window最后析构
}