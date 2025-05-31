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
	void Swap(Vec2& p0, Vec2& p1);
	std::vector<float> Interpolate(float i0, float d0, float i1, float d1);
	void DrawLine(Vec2 P0, Vec2 P1, Color);
	void DrawWireframeTriangle(Vec2 P0, Vec2 P1, Vec2 P2, Color color);
    void DrawFilledTriangle(Vec2 P0, Vec2 P1, Vec2 P2, Color color);


private:
    
	Painter& painter;
};