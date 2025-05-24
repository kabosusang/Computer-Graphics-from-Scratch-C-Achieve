#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include <memory>

struct Color
{
    Color(uint8_t r,uint8_t g,uint8_t b,uint8_t a):r(r),g(g),b(b),a(a){}
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class Painter {
public:
	static Painter& getInstance() {
		static Painter instance;
		return instance;
	}
	Painter(const Painter&) = delete;
	Painter& operator=(const Painter&) = delete;
	SDL_Renderer* GetRenderer() { return renderer.get(); }

//SDL Setting Painter
//设置颜色
void SetColor(Color color){SDL_SetRenderDrawColor(GetRenderer(),color.r,color.g,color.b,color.a);}
void Clear(){SDL_RenderClear(GetRenderer());}
void Present(){SDL_RenderPresent(GetRenderer());}

//RunPixelPen
void PutPixel(){}


private:
	Painter();
	~Painter();

private:
	std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> renderer;
};