#include "Tools/Shape.hpp"
#include "Tools/Vector.hpp"
#include <RayTracing/RayTracing.hpp>
#include <Tools/Color.hpp>
#include <cmath>
#include <optional>

RayTracing::RayTracing() :
		painter(Painter::getInstance()) {
	auto& canvas = Canvas::getInstance();
	canvaswidth_ = canvas.GetWindowW();
	canvasheight_ = canvas.GetWindowH();
	//全用单位1来表示
	viewportwight_ = viewportheight_ = 1;
	distance_ = 1;

	//场景圆添加
	scene_.AddSphere({ Vec3(0, -1, 3), 1, Color(255, 0, 0, 255) }); // 红色
	scene_.AddSphere({ Vec3(2, 0, 4), 1, Color(0, 0, 255, 255) }); // 蓝色
	scene_.AddSphere({ Vec3(-2, 0, 4), 1, Color(0, 255, 0, 255) }); // 绿色
}

void RayTracing::Renderer(float time) {
	{
        //清屏
        painter.Clear(Color{255,255,255,255});
		//遍历画布所有像素
		for (int x = -canvaswidth_ / 2; x < canvaswidth_ / 2; x++) {
			for (int y = -canvasheight_ / 2; y < canvasheight_ / 2; y++) {
				auto D = CanvasToViewport(Vec2(x, y));
				Color color = TraceRay(CameraPosition_, D, 1, 1000);
				painter.PutPixel(x, y, color);
			}
		}
		// 提交渲染
		painter.Present();
	}
}

Color RayTracing::TraceRay(Vec3 origin, Vec3 direction, int t_min, int t_max) {
	auto closet_t = 1000; //t的相交值 初始化设置为最大值
	std::optional<Sphere> closet_sphere;
	//遍历场景中的圆
	for (auto sphere : this->scene_.GetSpheres()) {
		auto [ts1, ts2] = IntersectRaySphere(CameraPosition_, direction, sphere);
		if (ts1 < closet_t && t_min < ts1 && ts1 < t_max) { // viewport和projection之间，并且找最近交点
			closet_t = ts1;
			closet_sphere = sphere;
		}
		if (ts2 < closet_t && t_min < ts2 && ts2 < t_max) { // viewport和projection之间，并且找最近交点
			closet_t = ts2;
			closet_sphere = sphere;
		}
	}

	if (closet_sphere) {
		return closet_sphere->color;
	} else {
		return Color(255, 255, 255, 255); //背景白色
	}
}

/**
 * @brief
 *  射线方程： 公式 P = O + t(V - O);
 *  圆方程：  (P -C) * (P - C) = r²
	两个方程带入化简如下 ↓
	t²(D * D) + 2t((O - C) * D) + ((O-C) * (O-C) - r²) = 0

 * 最终方程为： at² + bt + c = 0
	a = D * D
	b = 2((O - C) * D) CO：向量(从圆心到发射点) D：发射方向
	c = ((O-C) * (O-C) -r²)
* @param origion
 * @param direction
 * @param sphere
 * @return std::tuple<float, float>
 */
std::tuple<float, float> RayTracing::IntersectRaySphere(Vec3 origion, Vec3 direction, Sphere& sphere) {
	//向量CO (O-C)向量减法 后减前就是向量CO
	auto oc = Subtract(origion, sphere.center);

	auto k1 = DotProduct(direction, direction); //a
	auto k2 = 2 * DotProduct(oc, direction); //b
	auto k3 = DotProduct(oc, oc) - sphere.radius * sphere.radius; //c

	//计算b² - 4ac 计算方程有无解
	auto discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0) {
		return { -1, -1 };
	}

	//计算方程的两个跟
	auto t1 = (-k2 + std::sqrt(discriminant)) / (2 * k1);
	auto t2 = (-k2 - std::sqrt(discriminant)) / (2 * k1);
	return { t1, t2 };
}
