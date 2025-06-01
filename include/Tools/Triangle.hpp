#pragma once
#include "Color.hpp"
#include "Vector.hpp"


struct Vertex {
	float x;
	float y;
	float z;

    // 显式转换为 Vec3
    explicit operator Vec3() const {
        return Vec3{x, y, z};
    }
};

struct Pt {
	float x;
	float y;
	float h;
};

struct Triangle
{
    short v0;
    short v1;
    short v2;
    Color color;
};






