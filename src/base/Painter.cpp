#include <base/Canvas.hpp>
#include <base/Painter.hpp>

Painter::Painter() :
		renderer(SDL_CreateRenderer(Canvas::getInstance().GetWindow(), NULL), SDL_DestroyRenderer) {
	if (!renderer) {
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		SDL_Quit();
	}
}

Painter::~Painter() {
	renderer.reset();
}