#include "core/Event.hpp"
#include "core/Game.hpp"
#include <base/Canvas.hpp>
#include <base/Painter.hpp>
#include <RayTracing/RayTracing.hpp>
#include <Rasterization/Rasterization.hpp>

int main(int argc, char* argv[]) {
	//画布
	auto& canvas = Canvas::getInstance();
	//画笔
	auto& painter = Painter::getInstance();
    //光栅化着色器
    Rasterization Rasrenderer;

	//事件循环 和 GameLoop
	EventTrain train;
	Game game(train);

	//键盘事件
	train.subscribeEvent([&](SDL_Event& e) {
		if (e.type == SDL_EVENT_KEY_DOWN) {
			const char* keyName = SDL_GetScancodeName(e.key.scancode);
			//SDL_Log("Key pressed: %s (Scancode: %d)", keyName, e.key.scancode);

        }
	});
    
    train.subscribeFrame([&Rasrenderer](float deltaTime) {
		Rasrenderer.Renderer(deltaTime);
	});


	//渲染循环
	game.run();

	return 0;
}
