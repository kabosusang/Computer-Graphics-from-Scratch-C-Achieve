#include "Tools/Color.hpp"
#include "Tools/Vector.hpp"
#include "base/Painter.hpp"
#include <Rasterization/Rasterization.hpp>
#include <algorithm>
#include <cstdlib>
#include <vector>

Rasterization::Rasterization() :
		painter(Painter::getInstance()) {
}

void Rasterization::Renderer(float time) {
	painter.Clear(Color{ 255, 255, 255, 255 });
	DrawLine(Vec2(-200, -100), Vec2(240, 120), Color(0, 0, 0, 255));
	DrawLine(Vec2(-50, -200), Vec2(60, 240), Color(0, 0, 0, 255));

	painter.Present();
}

void Rasterization::DrawLine(Vec2 P0, Vec2 P1, Color color) {
	//y = f(x)
	// if (P0.x > P1.x) {
	// 	Swap(P0, P1);
	// }

	// //斜率
	// auto a = (P1.y - P0.y) / (P1.x - P0.x);
	// auto y = P0.y;
	// for (auto x = P0.x; x <= P1.x; x++) {
	// 	painter.PutPixel(x, y, color);
	// 	y = y + a;
	// }

	// x = f(y)
	// if (P0.y > P1.y)
	// {
	//     Swap(P0, P1);
	// }

	// auto a = (P1.x - P0.x) / (P1.y - P0.y);
	// auto x = P0.x;
	// for (auto y = P0.y; y <= P1.y; y++) {
	// 	painter.PutPixel(x, y, color);
	// 	x = x + a;
	// }

	if (std::abs(P1.x - P0.x) > std::abs(P1.y - P0.y)) {
		//直线偏向水平的情况
		//确保x0 < x1
		if (P0.x > P1.x) {
			Swap(P0, P1);
		}
		auto ys = Interpolate(P0.x, P0.y, P1.x, P1.y);
		for (auto x = P0.x; x <= P1.x; x++) {
			painter.PutPixel(x, ys[x - P0.x], color);
		}
	} else {
		//直线偏向垂直情况
		//确保y0 < y1
		if (P0.y > P1.y) {
			Swap(P0, P1);
		}
		auto ys = Interpolate(P0.y, P0.x, P1.y, P1.x);
		for (auto y = P0.y; y <= P1.y; y++) {
			painter.PutPixel(y, ys[y - P0.y], color);
		}
	}
}

//y = f(x) x = f(y)   ===>  d = f(i)
std::vector<float> Rasterization::Interpolate(float i0, float d0, float i1, float d1) {
	if (i0 == i1) {
		return { d0 }; //直接返回单元素列表
	}

	std::vector<float> values;
	auto a = (d1 - d0) / (i1 - i0);
	auto d = d0;
	for (int i = i0; i <= i1; i++) {
		values.emplace_back(d);
		d = d + a;
	}
	return values;
}

void Rasterization::Swap(Vec2& p0, Vec2& p1) {
	auto temp = p0;
	p0.x = p1.x;
	p0.y = p1.y;

	p1.x = temp.x;
	p1.y = temp.y;
}
