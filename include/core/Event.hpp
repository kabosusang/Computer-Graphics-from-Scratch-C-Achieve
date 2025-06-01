#pragma once
#include "SDL3/SDL_events.h"
#include <SDL3/SDL.h>
#include <functional>
#include <vector>

class EventTrain {
public:
	using EventHandler = std::function<void(SDL_Event&)>;
	using FrameHandler = std::function<void(float deltaTime)>;

	// 编译时检查handler是否可调用
	template <typename Handler>
	void subscribeEvent(Handler&& handler) {
		static_assert(std::is_invocable_v<Handler, SDL_Event&>,
				"Handler must be callable with SDL_Event&"); // 判断参数是否正确
		eventHandlers_.emplace_back(std::forward<Handler>(handler));
	}

	template <typename Handler>
	void subscribeFrame(Handler&& handler) {
		static_assert(
				std::is_invocable_v<Handler, float>,
				"Handler must be callable with float");
		frameHandlers_.emplace_back(std::forward<Handler>(handler));
	}

	void publishEvent(SDL_Event& event) {
		for (auto& handler : eventHandlers_) {
			handler(event);
		}
	}

	void publishFrame(float deltaTime) {
		for (auto& handler : frameHandlers_) {
			handler(deltaTime);
		}
	}

private:
	std::vector<EventHandler> eventHandlers_{};
	std::vector<FrameHandler> frameHandlers_{};
};