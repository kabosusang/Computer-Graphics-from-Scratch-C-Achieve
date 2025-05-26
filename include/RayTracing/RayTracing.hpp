#pragma once
#include "Tools/Shape.hpp"
#include <Raytracing/RayTracingScene.hpp>
#include <Tools/Vector.hpp>
#include <base/Painter.hpp>
#include <tuple>

class RayTracing {
public:
	RayTracing();

	/**
	 * @brief 画布到Viewport(画框)转换
	 *  CanvasToViewport(x, y){
	 *    return (x * Vw / Cw, y * Vh / Ch, d)
	 * @param point x,y
	 * @param wh wh.x = Vw wh.y = Vh wh.z = Cw  wh.w = Ch
	 * @param d  d
	 * @return Vec3
	 */
	static Vec3 CanvasToViewport(Vec2 point, Vec4 wh, float d) {
		return { point.x * wh.x / wh.z, point.y * wh.y / wh.w, d };
	}
    //主渲染逻辑
	void Renderer(float time);
    void AddCameraX(){
        CameraPosition_.x++;
    }

    void ChangeCameraPosition(SDL_Scancode code);
private:
	Vec3 CanvasToViewport(Vec2 point) {
		return { point.x * viewportwight_ / canvaswidth_, point.y * viewportheight_ / canvasheight_, distance_ };
	}
	/**
	 * @brief
	 * 公式 P = O + t(V - O); //射线公式
	 * @param O 相机坐标  (射线起点)
	 * @param D Direction (射线方向)
	 * @param t_min t最小取值
	 * @param t_max t最大取值
	 * @return Color
	 */
	Color TraceRay(Vec3 O, Vec3 D, int t_min, int t_max,int depth);
	std::tuple<float, float> IntersectRaySphere(Vec3 origion, Vec3 direction, Sphere& sphere);
    
    //4-1 封装射线函数 方便复用
    std::tuple<Sphere,float> ClosestIntersection(Vec3 origion, Vec3 direction,float min_t,float max_t);
    
    //计算光照
    float ComputeLighting(Vec3 P,Vec3 N,Vec3 view,int specular);
    
private:
	Vec3 CameraPosition_{ 0, 0, 0 };
	int canvaswidth_, canvasheight_;
	int viewportwight_, viewportheight_;

	float distance_;
	Painter& painter;


    //const No Zero
    const float EPSILON = 0.001;

    //const 
    const int Depth = 1; //光线反射深度
private:
	//Scene
	RayTracingScene scene_;
};