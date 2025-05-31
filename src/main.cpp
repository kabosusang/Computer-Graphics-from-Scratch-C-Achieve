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
	//光线追踪渲染器
	RayTracing rayrenderer;

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
            //rayrenderer.ChangeCameraPosition(e.key.scancode);
        
        }
	});
    
	//渲染事件
	// train.subscribeFrame([&rayrenderer](float deltaTime) {
	// 	//rayrenderer.Renderer(deltaTime);
    //     //rayrenderer.ParallelRender(deltaTime); //简单多线程渲染
	// });

    train.subscribeFrame([&Rasrenderer](float deltaTime) {
		Rasrenderer.Renderer(deltaTime);
	});


	//渲染循环
	game.run();

	return 0;
}
