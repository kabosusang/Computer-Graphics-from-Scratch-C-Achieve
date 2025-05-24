#include <core/Game.hpp>

void Game::run() {
	running_ = true;
	auto lastTime = std::chrono::high_resolution_clock::now();

	while (running_) {
		//计算时间差
		auto now = std::chrono::high_resolution_clock::now();
		float deltaTime = std::chrono::duration<float>(now - lastTime).count();
		lastTime = now;

		//Event
		// 处理输入事件
		pollEvents();

		// 更新游戏状态
		bus_.publishFrame(deltaTime);

		// 控制帧率
		limitFrameRate(deltaTime);
	}
}

void Game::pollEvents() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT) {
			running_ = false;
		}
		bus_.publishEvent(e);
	}
}

void Game::limitFrameRate(float deltaTime) {
	const float targetFrameTime = 1.0f / targetFrameRate; // 60 FPS
	if (deltaTime < targetFrameTime) {
		SDL_Delay(static_cast<Uint32>((targetFrameTime - deltaTime) * 1000));
	}
}