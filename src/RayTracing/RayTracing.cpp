#include "SDL3/SDL_scancode.h"
#include "Tools/Light.hpp"
#include "Tools/Shape.hpp"
#include "Tools/Vector.hpp"
#include <RayTracing/RayTracing.hpp>
#include <Tools/Color.hpp>
#include <algorithm>
#include <limits>
#include <mutex>
#include <iostream>
#include <thread>

RayTracing::RayTracing() :
		painter(Painter::getInstance()) {
	auto& canvas = Canvas::getInstance();
	canvaswidth_ = canvas.GetWindowW();
	canvasheight_ = canvas.GetWindowH();
	//全用单位1来表示
	viewportwight_ = viewportheight_ = 1;
	distance_ = 1;

	//场景圆添加
	scene_.AddSphere({ Vec3(0, -1, 3), 1, Color(255, 0, 0, 255), 500, 0.2f }); // 红色 闪亮
	scene_.AddSphere({ Vec3(2, 0, 4), 1, Color(0, 0, 255, 255), 500, 0.3f }); // 蓝色 闪亮
	scene_.AddSphere({ Vec3(-2, 0, 4), 1, Color(0, 255, 0, 255), 10, 0.4f }); // 绿色 略微善良
	scene_.AddSphere({ Vec3(0, -5001, 0), 5000, Color(255, 255, 0, 255), 1000, 0.5f }); // 黄色很大的圆 异常善良

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
				Color color = TraceRay(CameraPosition_, D, 1, 1000, this->Depth);
				painter.PutPixel(x, y, color);
			}
		}
		// 提交渲染
		painter.Present();
	}
}

Color RayTracing::TraceRay(Vec3 origin, Vec3 direction, int t_min, int t_max, int depth) {
	//返回相交的圆型和t的值
	auto [intersection1, intersection2] = ClosestIntersection(origin, direction, t_min, t_max);
	//如果t是空的
	if (!intersection2) {
		return { 0, 0, 0, 255 }; //返回白色背景
	}

	auto closest_sphere = intersection1;
	auto closet_t = intersection2;
	//射线和圆有交点
	//交点
	auto point = VAdd(origin, VMultiply(closet_t, direction));
	//法线(从圆心射出)
	auto normal = VSubtract(point, closest_sphere.center);
	//法线归一化
	normal = VNormalize(normal);

	//计算光照
	auto view = VMultiply(-1, direction);
	float lighting = ComputeLighting(point, normal, view, closest_sphere.specular);
	//Fix 反射光变成黑球
	lighting = std::clamp(lighting, 0.0f, 1.0f); // 确保光照在 [0,1] 范围
	auto local_color = VMultiply(lighting, ColorToVec3(closest_sphere.color));

	//如果不能反射 CPU单核渲染只有两帧(-)
	if (closest_sphere.reflective <= 0 || depth <= 0) {
		return Vec3ToColor(local_color);
	}

	//反射光线
	auto reflected_ray = ReflectRay(view, normal);
	auto offset_origin = VAdd(point, VMultiply(EPSILON, normal));
	auto reflected_color = TraceRay(offset_origin, reflected_ray, EPSILON, 1000, depth - 1);
	//auto reflected_color = TraceRay(point, reflected_ray, EPSILON, 1000, depth - 1);

	auto color = VAdd(
			VMultiply(1 - closest_sphere.reflective, local_color),
			VMultiply(closest_sphere.reflective, ColorToVec3(reflected_color)));
	return Vec3ToColor(VClamp(color)); // 新增颜色截断
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
			float t_max{};
			switch (light.type) {
				case LightType::directional: {
					vec_l = light.direction.value();
					t_max = std::numeric_limits<float>::max();
				} break;
				case LightType::point: {
					vec_l = VSubtract(light.position.value(), Point);
					t_max = 1.0f; //设置点光源的t最大值
				} break;
				default:
					break;
			}

			//检查阴影
			//auto [s1, t1] = ClosestIntersection(Point, vec_l, EPSILON, t_max);
            auto isShadow = BClosestIntersection(Point, vec_l, EPSILON, t_max);
			//如果有阴影跳过像素
			if (isShadow) {
				continue;
			}
			//漫反射
			auto n_dot_l = VDotProduct(Normal, vec_l);
			if (n_dot_l > 0) {
				//最终方程计算
				//Ii * (N*L) / 丨N丨丨L丨
				intensity += light.intensity * n_dot_l / (length_n * VLength(vec_l));
			}
			//镜面反射
			if (specular > 0) {
				auto vec_r = ReflectRay(vec_l, Normal);
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

//4-1 添加阴影增加
std::tuple<Sphere, float> RayTracing::ClosestIntersection(Vec3 origion, Vec3 direction, float t_min, float t_max) {
	auto closet_t = std::numeric_limits<float>::max(); //t的相交值 初始化设置为最大值
	std::optional<Sphere> closet_sphere;
	//遍历场景中的圆
	for (auto sphere : this->scene_.GetSpheres()) {
		auto [ts1, ts2] = IntersectRaySphere(origion, direction, sphere);
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
		return { {}, {} }; //返回空
	}
	return { closet_sphere.value(), closet_t };
}


bool RayTracing::BClosestIntersection(Vec3 origion, Vec3 direction, float t_min, float t_max) {
	auto closet_t = std::numeric_limits<float>::max(); //t的相交值 初始化设置为最大值
	std::optional<Sphere> closet_sphere;
	//遍历场景中的圆
	for (auto sphere : this->scene_.GetSpheres()) {
		auto [ts1, ts2] = IntersectRaySphere(origion, direction, sphere);
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
		return false;
	}
	return true;
}





void RayTracing::ChangeCameraPosition(SDL_Scancode code) {
	if (code == SDL_SCANCODE_W) {
		CameraPosition_.z++;
	}

	if (code == SDL_SCANCODE_A) {
		CameraPosition_.x--;
	}

	if (code == SDL_SCANCODE_S) {
		CameraPosition_.z--;
	}

	if (code == SDL_SCANCODE_D) {
		CameraPosition_.x++;
	}

	if (code == SDL_SCANCODE_Q) {
		CameraPosition_.y++;
	}

	if (code == SDL_SCANCODE_E) {
		CameraPosition_.y--;
	}
}

/**
 * @brief 
 * 类似手机上的渲染 分块渲染最后合并
 * @param startX 
 * @param endX 
 * @param startY 
 * @param endY 
 */
void RayTracing::RenderTile(int startX, int endX, int startY, int endY) {
	for (int y = startY; y < endY; ++y) {
		for (int x = startX; x < endX; ++x) {
			auto D = CanvasToViewport(Vec2(x, y));
			Color color = TraceRay(CameraPosition_, D, 1, 1000, Depth);
			std::lock_guard<std::mutex> lock(pixelMutex_);
			painter.PutPixel(x, y, color);
		}
	}
}

void RayTracing::ParallelRender(float time) {
    std::cout << "FPS: " << 1.0f / time << std::endl;

	painter.Clear(Color{ 255, 255, 255, 255 });

	const int tileSize = 128; // 128 x 128 像素块
	std::vector<std::thread> threads;

	for (int y = -canvasheight_ / 2; y < canvasheight_ / 2; y += tileSize) {
		for (int x = -canvaswidth_ / 2; x < canvaswidth_ / 2; x += tileSize) {
			threads.emplace_back([=]() {
				this->RenderTile(x, std::min(x + tileSize, canvaswidth_ / 2),
						y, std::min(y + tileSize, canvasheight_ / 2));
			});
		}
	}

	for (auto& t : threads) {
		t.join();
	}
	painter.Present();
}
