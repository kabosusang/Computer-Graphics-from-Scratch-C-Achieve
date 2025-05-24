#pragma once
#include <chrono>
#include <core/Event.hpp>

class Game {
public:
	Game(EventTrain& bus) :
			bus_(bus) {}

	void run();
	inline void stop() { running_ = false; }
	inline void SetFrameRate(int rate) { targetFrameRate = rate; }
private:
	void pollEvents();

	void limitFrameRate(float deltaTime);
	float targetFrameRate = 60.0f;
	EventTrain& bus_;
	bool running_ = false;
};