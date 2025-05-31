#include "Tools/Color.hpp"
#include "Tools/Vector.hpp"
#include "base/Painter.hpp"
#include <Rasterization/Rasterization.hpp>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <vector>

Rasterization::Rasterization() :
		painter(Painter::getInstance()) {
}

void Rasterization::Renderer(float time) {
	painter.Clear(Color{ 255, 255, 255, 255 });
    DrawFilledTriangle(Vec2(-200, -250), Vec2(20, 250), Vec2(200, 50), { 0, 255, 0, 255 });
	DrawWireframeTriangle(Vec2(-200, -250), Vec2(20, 250), Vec2(200, 50), { 0, 0, 0, 255 });
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
		auto xs = Interpolate(P0.y, P0.x, P1.y, P1.x);
		for (auto y = P0.y; y <= P1.y; y++) {
			painter.PutPixel(xs[y - P0.y], y, color);
		}
	}
}

void Rasterization::DrawWireframeTriangle(Vec2 P0, Vec2 P1, Vec2 P2, Color color) {
	DrawLine(P0, P1, color);
	DrawLine(P1, P2, color);
	DrawLine(P0, P2, color);
}

//y = f(x) x = f(y)   ===>  d = f(i)
std::vector<float> Rasterization::Interpolate(float i0, float d0, float i1, float d1) {
	if (i0 == i1) {
		return { d0 };
	}

	std::vector<float> values;
	float a = (d1 - d0) / (i1 - i0);
	float d = d0;

	// 使用 float 循环变量，步进 1.0f
	for (float i = i0; i <= i1; i += 1.0f) {
		values.push_back(d);
		d += a;
	}
	return values;
}

void Rasterization::Swap(Vec2& p0, Vec2& p1) {
	Vec2 temp = p0;
	p0 = p1;
	p1 = temp;
}

void Rasterization::DrawFilledTriangle(Vec2 P0, Vec2 P1, Vec2 P2, Color color) {
	// 1.对于三个点Y值排序
	if (P1.y < P0.y) {
		Swap(P1, P0);
	}
	if (P2.y < P0.y) {
		Swap(P2, P0);
	}
	if (P2.y < P1.y) {
		Swap(P2, P1);
	}

	//2.计算三角形各条边的x坐标
	//两条短边
	auto x01 = Interpolate(P0.y, P0.x, P1.y, P1.x);
	auto x12 = Interpolate(P1.y, P1.x, P2.y, P2.x);
	//长边
	auto x02 = Interpolate(P0.y, P0.x, P2.y, P2.x);
	//3.拼接短边数组
	x01.pop_back(); //去除最后一个(因为x01这条边和x12这条边最后一个点有重合)
	x01.insert(x01.end(), x12.begin(), x12.end());
	auto& x012 = x01; //存储两条短边

	//4. 决定哪条边是左侧边 哪条是右侧边
	auto m = std::floor(x012.capacity() / 2);

	auto& x_left = (x02.at(m) < x012.at(m)) ? x02 : x012;
	auto& x_right = (x02.at(m) < x012.at(m)) ? x012 : x02;

	//5. 绘制水平直线段
	for (auto y = P0.y; y <= P2.y; y++) {
		for (auto x = x_left[y - P0.y]; x <= x_right[y - P0.y]; x++)
        {
            painter.PutPixel(x, y, color);
        }
	}
}