#include <base/Canvas.hpp>
#include <base/Painter.hpp>

Painter::Painter() :canvas(Canvas::getInstance()),
		renderer(SDL_CreateRenderer(Canvas::getInstance().GetWindow(), NULL), SDL_DestroyRenderer) {
	if (!renderer) {
		SDL_Log("Unable to create renderer: %s", SDL_GetError());
		SDL_Quit();
	}
}

Painter::~Painter() {
	renderer.reset();
}

void Painter::PutPixel(float x, float y,Color color) {
	SetColor(color);
	//ChangeCorrdinate
    
	auto x1 = static_cast<int>(canvas.GetWindowW() / 2) + x;
	auto y1 = static_cast<int>(canvas.GetWindowH()  / 2) - y;
    
	SDL_RenderPoint(GetRenderer(), x1, y1);
}
