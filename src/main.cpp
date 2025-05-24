#include "core/Event.hpp"
#include "core/Game.hpp"
#include <base/Canvas.hpp>
#include <base/Painter.hpp>

void RunRect(float deltime) {
    auto& painter = Painter::getInstance();
    auto renderer = painter.GetRenderer();
	// 清屏
    painter.SetColor(Color(30,30,30,255));//设置画笔颜色
	painter.Clear();

	// 渲染逻辑（示例：移动方块）
	static float x = 0;
	x += 100 * deltime; // 每秒移动100像素
	if (x > 800) {
		x = 0;
	}

	SDL_FRect rect{ x, 300, 50, 50 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &rect);

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
	train.subscribeFrame(RunRect);
    
    
    //渲染循环
	game.run();

	return 0;
}
