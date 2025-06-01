#include "Tools/Color.hpp"
#include "Tools/Triangle.hpp"
#include "Tools/Vector.hpp"
#include "base/Painter.hpp"
#include <Rasterization/Rasterization.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

std::vector<Vertex> vertexes = {
	Vertex(1, 1, 1),
	Vertex(-1, 1, 1),
	Vertex(-1, -1, 1),
	Vertex(1, -1, 1),
	Vertex(1, 1, -1),
	Vertex(-1, 1, -1),
	Vertex(-1, -1, -1),
	Vertex(1, -1, -1)
};

auto RED = Color{ 255, 0, 0 };
auto GREEN = Color{ 0, 255, 0 };
auto BLUE = Color{ 0, 0, 255 };
auto YELLOW = Color{ 255, 255, 0 };
auto PURPLE = Color{ 255, 0, 255 };
auto CYAN = Color{ 0, 255, 255 };

std::vector<Triangle> triangles = {
	Triangle(0, 1, 2, RED),
	Triangle(0, 2, 3, RED),
	Triangle(4, 0, 3, GREEN),
	Triangle(4, 3, 7, GREEN),
	Triangle(5, 4, 7, BLUE),
	Triangle(5, 7, 6, BLUE),
	Triangle(1, 5, 6, YELLOW),
	Triangle(1, 6, 2, YELLOW),
	Triangle(4, 5, 1, PURPLE),
	Triangle(4, 1, 0, PURPLE),
	Triangle(2, 6, 7, CYAN),
	Triangle(2, 7, 3, CYAN)
};

Rasterization::Rasterization() :
		painter(Painter::getInstance()),
		canvas(Canvas::getInstance()) {
	for (auto i = 0; i < vertexes.size(); i++) {
		vertexes[i].x -= 1.5;
		vertexes[i].z += 7;
	}
}

void Rasterization::Renderer(float time) {
	//std::cout << "FPS: " << 1.0f / time << std::endl;
	painter.Clear(Color{ 255, 255, 255, 255 });
    RenderObject(vertexes, triangles);
	painter.Present();
}

void Rasterization::RenderTriangle(Triangle& triangle, std::vector<Vec2>& projected) {
	DrawWireframeTriangle(projected[triangle.v0],
			projected[triangle.v1],
			projected[triangle.v2],
			triangle.color);
}

void Rasterization::RenderObject(std::vector<Vertex>& vertexes, std::vector<Triangle>& triangles) {
	auto projected = std::vector<Vec2>();
	for (auto i = 0; i < vertexes.capacity(); i++) {
		projected.push_back(ProjectVertex((Vec3)vertexes[i]));
	}
	for (auto i = 0; i < triangles.capacity(); i++) {
		RenderTriangle(triangles[i], projected);
	}
}

void Rasterization::DrawLine(Vec2 P0, Vec2 P1, Color color) {
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

void Rasterization::DrawLine(Vec3 P0, Vec3 P1, Color color) {
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

void Rasterization::DrawWireframeTriangle(Vec3 P0, Vec3 P1, Vec3 P2, Color color) {
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

void Rasterization::Swap(Vec3& p0, Vec3& p1) {
	Vec3 temp = p0;
	p0 = p1;
	p1 = temp;
}

void Rasterization::DrawFilledTriangle(Vec3 P0, Vec3 P1, Vec3 P2, Color color) {
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
	// //两条短边
	//边01     P0.z = P0.h
	auto x01 = Interpolate(P0.y, P0.x, P1.y, P1.x);
	auto h01 = Interpolate(P0.y, P0.z, P1.y, P1.z);
	//边12
	auto x12 = Interpolate(P1.y, P1.x, P2.y, P2.x);
	auto h12 = Interpolate(P1.y, P1.z, P2.y, P2.z);

	//长边
	//边02
	auto x02 = Interpolate(P0.y, P0.x, P2.y, P2.x);
	auto h02 = Interpolate(P0.y, P0.z, P2.y, P2.z);

	//3.拼接短边数组
	x01.pop_back(); //去除最后一个(因为x01这条边和x12这条边最后一个点有重合)
	x01.insert(x01.end(), x12.begin(), x12.end());
	auto& x012 = x01; //存储两条短边

	//确定短边的h
	h01.pop_back(); //去除最后一个(因为x01这条边和x12这条边最后一个点有重合)
	h01.insert(h01.end(), h12.begin(), h12.end());
	auto& h012 = h01; //存储两条短边

	//4. 决定哪条边是左侧边 哪条是右侧边
	auto m = std::floor(x012.capacity() / 2);

	// 定义选择函数
	auto get_boundaries = [&]() -> std::tuple<std::vector<float>&, std::vector<float>&, std::vector<float>&, std::vector<float>&> {
		if (x02.at(m) < x012.at(m)) {
			return { x02, x012, h02, h012 };
		} else {
			return { x012, x02, h012, h02 };
		}
	};

	auto [x_left, x_right, h_left, h_right] = get_boundaries(); // C++17 结构化绑定

	//5. 绘制水平直线段
	for (auto y = P0.y; y <= P2.y; y++) {
		// 边界检查
		int idx = y - static_cast<int>(P0.y);
		if (idx < 0 || idx >= x_left.size() || idx >= x_right.size()) {
			continue;
		}

		// 获取当前行的左右边界（强制取整）
		int xl = static_cast<int>(x_left.at(idx));
		int xr = static_cast<int>(x_right.at(idx));

		// 插值生成颜色因子线段
		std::vector<float> h_segment = Interpolate(
				xl, h_left.at(idx),
				xr, h_right.at(idx));

		for (auto x = x_left.at(y - P0.y); x <= x_right.at(y - P0.y); x++) {
			// 混合颜色：color * h_segment[x - xl]
			Color blended_color = Vec3ToColor(VMultiply(h_segment[x - xl], ColorToVec3(color)));
			painter.PutPixel(x, y, blended_color);
		}
	}
}

Vec2 Rasterization::ViewportToCanvas(Vec2 p2d) {
	return Vec2(
			p2d.x * canvas.GetWindowW() / viewport_size,
			p2d.y * canvas.GetWindowH() / viewport_size);
}

Vec2 Rasterization::ProjectVertex(Vec3 v) {
	return ViewportToCanvas(
			{ v.x * projection_plane_z / v.z,
					v.y * projection_plane_z / v.z });
}
