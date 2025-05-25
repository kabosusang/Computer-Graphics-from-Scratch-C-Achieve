#include "core/Event.hpp"
#include "core/Game.hpp"
#include <RayTracing/RayTracing.hpp>
#include <base/Canvas.hpp>
#include <base/Painter.hpp>

int main(int argc, char* argv[]) {
	//画布
	auto& canvas = Canvas::getInstance();
	//画笔
	auto& painter = Painter::getInstance();
	//光线追踪渲染器
	RayTracing renderer;

	//事件循环 和 GameLoop
	EventTrain train;
	Game game(train);

	//键盘事件
	train.subscribeEvent([&](SDL_Event& e) {
		if (e.type == SDL_EVENT_KEY_DOWN) {
			const char* keyName = SDL_GetScancodeName(e.key.scancode);
			SDL_Log("Key pressed: %s (Scancode: %d)", keyName, e.key.scancode);
            renderer.AddCameraX();
        }
	});
    
	//渲染事件
	train.subscribeFrame([&renderer](float deltaTime) {
		renderer.Renderer(deltaTime);
	});

	//渲染循环
	game.run();

	return 0;
}
