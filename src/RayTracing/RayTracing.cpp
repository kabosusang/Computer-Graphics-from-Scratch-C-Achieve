#include "Tools/Light.hpp"
#include "Tools/Shape.hpp"
#include "Tools/Vector.hpp"
#include <RayTracing/RayTracing.hpp>
#include <Tools/Color.hpp>
#include <algorithm>
#include <iostream>
#include <limits>

RayTracing::RayTracing() :
		painter(Painter::getInstance()) {
	auto& canvas = Canvas::getInstance();
	canvaswidth_ = canvas.GetWindowW();
	canvasheight_ = canvas.GetWindowH();
	//全用单位1来表示
	viewportwight_ = viewportheight_ = 1;
	distance_ = 1;

	//场景圆添加
	scene_.AddSphere({ Vec3(0, -1, 3), 1, Color(255, 0, 0, 255), 500 }); // 红色 闪亮
	scene_.AddSphere({ Vec3(2, 0, 4), 1, Color(0, 0, 255, 255), 500 }); // 蓝色 闪亮
	scene_.AddSphere({ Vec3(-2, 0, 4), 1, Color(0, 255, 0, 255), 10 }); // 绿色 略微善良
	scene_.AddSphere({ Vec3(0, -5001, 0), 5000, Color(255, 255, 0, 255), 1000 }); // 黄色很大的圆 异常善良

	//场景光照添加
	//环境光
	scene_.AddLight({ LightType::ambient, 0.2f, std::nullopt, std::nullopt });
	//点光源
	scene_.AddLight({ LightType::point, 0.6f, Vec3(2, 1, 0), std::nullopt });
	//平行光
	scene_.AddLight({ LightType::directional, 0.2f, std::nullopt, Vec3(1, 4, 4) });
}

void RayTracing::Renderer(float time) {
	{
		//std::cout << "FPS: " << 1.0f / time << std::endl; //我CPU只有5帧数
		//清屏
		painter.Clear(Color{ 255, 255, 255, 255 });
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
	auto closet_t = std::numeric_limits<float>::max(); //t的相交值 初始化设置为最大值
	std::optional<Sphere> closet_sphere;
	//遍历场景中的圆
	for (auto sphere : this->scene_.GetSpheres()) {
		auto [ts1, ts2] = IntersectRaySphere(origin, direction, sphere);
		if (ts1 < closet_t && t_min < ts1 && ts1 < t_max) { // viewport和projection之间，并且找最近交点
			closet_t = ts1;
			closet_sphere = sphere;
		}
		if (ts2 < closet_t && t_min < ts2 && ts2 < t_max) { // viewport和projection之间，并且找最近交点
			closet_t = ts2;
			closet_sphere = sphere;
		}
	}
	//射线和圆无交点
	if (!closet_sphere.has_value()) {
		return Color(255, 255, 255, 255); //背景白色
	}

	//射线和圆有交点
	//交点
	auto point = VAdd(origin, VMutiply(closet_t, direction));
	//法线(从圆心射出)
	auto normal = VSubtract(point, closet_sphere->center);
	//法线归一化
	normal = VNormalize(normal);

	//计算光照
	auto view = VMutiply(-1, direction);
	float lighting = ComputeLighting(point, normal, view, closet_sphere->specular);
	//Fix 反射光变成黑球
    lighting = std::clamp(lighting, 0.0f, 1.0f); // 确保光照在 [0,1] 范围
    return Vec3ToColor(VMutiply(lighting, ColorToVec3(closet_sphere->color)));
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
	auto oc = VSubtract(origion, sphere.center);

	auto k1 = VDotProduct(direction, direction); //a
	auto k2 = 2 * VDotProduct(oc, direction); //b
	auto k3 = VDotProduct(oc, oc) - sphere.radius * sphere.radius; //c

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

/**
 * @brief
 *  //  光照方程
	// 漫反射方程+ 镜面反射方程
    // 漫反射方程-> Ip = Ia + Ii * (N*L) / 丨N丨丨L丨 
	//  Ia 环境光强度
	//  Ii 灯光强度
	// (N*L) / 丨N丨丨L丨 夹角的余弦
    //----------------------------------
    //镜面反射方程
    //[Ri * V / 丨Ri丨丨V丨] 的S平方
    //S Specular
    //Ri 反射方向
    //V 看向方向 
 * @param Point
 * @param Normal
 * @return float
 */
float RayTracing::ComputeLighting(Vec3 Point, Vec3 Normal, Vec3 view, int specular) {
	auto intensity = 0.0f; //光强
	auto length_n = VLength(Normal); // 法线长度
	auto length_v = VLength(view); //视线长度

	for (auto light : this->scene_.GetLights()) {
		if (light.type == LightType::ambient) {
			intensity += light.intensity;
		} else {
			auto vec_l = Vec3{}; //光照方向
			switch (light.type) {
				case LightType::directional:
					vec_l = light.direction.value();
					break;
				case LightType::point:
					vec_l = VSubtract(light.position.value(), Point);
					break;
				default:
					break;
			}
			auto n_dot_l = VDotProduct(Normal, vec_l);
			if (n_dot_l > 0) {
				//最终方程计算
				//Ii * (N*L) / 丨N丨丨L丨
				intensity += light.intensity * n_dot_l / (length_n * VLength(vec_l));
			}
            
			if (specular > 0) { 
				auto vec_r = VSubtract(VMutiply(2.0 * VDotProduct(Normal, vec_l), Normal), vec_l);
				auto r_dot_v = VDotProduct(vec_r, view);
				if (r_dot_v > 0) {
					float denominator = VLength(vec_r);
					if (denominator > 1e-6f) {
						intensity += light.intensity * std::pow(r_dot_v / (denominator * length_v), specular);
					}
				}
			}
		}
	}
	return intensity;
}
