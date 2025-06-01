#pragma once
#include <Tools/Vector.hpp>
#include <Tools/Triangle.hpp>
#include <Tools/Model.hpp>
#include <Tools/Vertex.hpp>
#include <Tools/MaTrix.hpp>
#include <Tools/Camera.hpp>

#include "base/Canvas.hpp"
#include <base/Painter.hpp>
#include <vector>

class Rasterization {
public:
	Rasterization();
	void Renderer(float time);

private:
	void Swap(Vec2& p0, Vec2& p1);
	void Swap(Vec3& p0, Vec3& p1);
	std::vector<float> Interpolate(float i0, float d0, float i1, float d1);

	//Chapter8 Add h
	void DrawLine(Vec2 P0, Vec2 P1, Color);
	void DrawLine(Vec3 P0, Vec3 P1, Color);
	void DrawWireframeTriangle(Vec3 P0, Vec3 P1, Vec3 P2, Color color);
	void DrawWireframeTriangle(Vec2 P0, Vec2 P1, Vec2 P2, Color color);
    void DrawFilledTriangle(Vec3 P0, Vec3 P1, Vec3 P2, Color color);

//Scene
private:
    void RenderScene(Camera& camera,std::vector<Instance>& instance);
    void RenderInstance(Instance& instance);
    void RenderModel(Model&  instance,Mat4x4 transform);

    void RenderObject(std::vector<Vertex>& vertexes,std::vector<Triangle>& triangles);
    void RenderTriangle(Triangle& triangle,std::vector<Vec2>&);
private:
	//P'x P'y 转换为画布上的点
	Vec2 ViewportToCanvas(Vec2 p2d);
	//Project 把3D坐标空间的点投影到Viewport上面(因为d确定所以用Vector2)
	Vec2 ProjectVertex(Vec3 vertex);
	Vec2 ProjectVertex(Vec4 vertex);
private:
	Painter& painter;
	Canvas& canvas;
	//表示Viewport和Canvas是一比一
	float viewport_size = 1;
	//d = 1 表示Vieport视口的距离
	float projection_plane_z = 1;
	Vec3 cameraposition_{ 0, 0, 0 };
};