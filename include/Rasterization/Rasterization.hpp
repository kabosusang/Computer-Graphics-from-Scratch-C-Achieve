#pragma once
#include "Tools/Vector.hpp"
#include <Tools/Color.hpp>
#include <base/Painter.hpp>
#include <vector>

class Rasterization {
public:
	Rasterization();
	void Renderer(float time);

private:
	void Swap(Vec3& p0, Vec3& p1);
	std::vector<float> Interpolate(float i0, float d0, float i1, float d1);
	
    //Chapter8 Add h
	void DrawLine(Vec2 P0, Vec2 P1, Color);
	void DrawWireframeTriangle(Vec3 P0, Vec3 P1, Vec3 P2, Color color);
	void DrawLine(Vec3 P0, Vec3 P1, Color);
    void DrawFilledTriangle(Vec3 P0, Vec3 P1, Vec3 P2, Color color);
    

private:
    
	Painter& painter;
};