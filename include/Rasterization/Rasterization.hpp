#pragma once
#include "Tools/Vector.hpp"
#include <Tools/Color.hpp>
#include <base/Painter.hpp>

class Rasterization {
public:
	Rasterization();
	void Renderer(float time);

private:
	void DrawLine(Vec2 P0, Vec2 P1, Color);

	Painter& painter;
};