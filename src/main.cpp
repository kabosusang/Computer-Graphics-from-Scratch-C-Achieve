#include "core/Event.hpp"
#include "core/Game.hpp"
#include <base/Canvas.hpp>
#include <base/Painter.hpp>

void RunPixel(float deltime) {
    auto& painter = Painter::getInstance();
    //清屏
	painter.Clear(Color(255,255,255,255));

    //画一个像素
    painter.PutPixel(0,0,Color{255,0,0,255});
    
	// 提交渲染
	painter.Present();
}

int main(int argc, char* argv[]) {
	//画布
	auto& canvas = Canvas::getInstance();
	//画笔
	auto& painter = Painter::getInstance();

	//事件循环 和 GameLoop
	EventTrain train;
	Game game(train);
	
    //键盘事件
	train.subscribeEvent([](SDL_Event& e) {
		if (e.type == SDL_EVENT_KEY_DOWN) {
			SDL_Log("Key pressed: %d", e.key.scancode);
		}
	});
    //渲染事件
	train.subscribeFrame(RunPixel);
    
    
    //渲染循环
	game.run();

	return 0;
}
